#!/usr/bin/env python3
"""编译 SysY，用 llvm-as 验证原始 IR，再注入运行库并用 lli 校验输出和退出码。"""

from __future__ import annotations

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
import time
from collections import Counter
from concurrent.futures import ThreadPoolExecutor, as_completed
from dataclasses import asdict, dataclass
from difflib import unified_diff
from pathlib import Path


TASK_ROOT = Path(__file__).resolve().parent
FUNCTION_HEADER = re.compile(
    r'^\s*(declare|define)\s+[^@\n]*@("(?:[^"\\]|\\.)+"|[-a-zA-Z$._0-9]+)\s*\('
)


@dataclass(frozen=True)
class CaseResult:
    case: str
    status: str
    compile_seconds: float = 0.0
    run_seconds: float = 0.0
    detail: str = ""


def positive_int(value: str) -> int:
    number = int(value)
    if number <= 0:
        raise argparse.ArgumentTypeError("必须为正整数")
    return number


def positive_float(value: str) -> float:
    number = float(value)
    if not 0 < number < float("inf"):
        raise argparse.ArgumentTypeError("必须为有限正数")
    return number


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=__doc__,
        epilog=("默认递归运行全部 .sy；同名 .in 可选，.out 必须存在。"
                ".out 最后一行是退出码，其余内容按行精确比较（忽略最终换行差异，"
                "不忽略空格，也不使用浮点容差）。默认自动清理产物。"
                "脚本退出码：0 全通过，1 存在失败，2 环境或参数错误。"),
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    parser.add_argument("--compiler", type=Path,
                        default=TASK_ROOT / "build/compiler_middle_end", help="前中端可执行文件")
    parser.add_argument("--test-root", type=Path,
                        default=TASK_ROOT.parent / "testcases26", help="递归查找用例的根目录")
    parser.add_argument("--runtime", type=Path,
                        default=TASK_ROOT / "runtime/runtime.ll", help="要前置注入的运行库 IR")
    parser.add_argument("--lli", help="lli 可执行文件路径或命令名；自动优先使用 LLVM 15+")
    parser.add_argument("--llvm-as", help="llvm-as 路径或命令名；须与 lli 主版本一致，默认自动查找")
    parser.add_argument("--opt-level", choices=("0", "1", "2"), default="1",
                        help="传给前中端编译器的优化等级")
    parser.add_argument("--jobs", type=positive_int,
                        default=min(os.cpu_count() or 1, 4), help="并行用例数")
    parser.add_argument("--compile-timeout", type=positive_float, default=60.0,
                        help="单个用例的编译时限（秒）")
    parser.add_argument("--verify-timeout", type=positive_float, default=60.0,
                        help="单个用例原始 IR 验证时限（秒，不执行程序）")
    parser.add_argument("--timeout", type=positive_float, default=60.0,
                        help="单次 lli 执行时限（秒，含 JIT）")
    parser.add_argument("--filter", action="append", default=[],
                        help="只运行相对路径包含该字符串的用例，可重复（取并集）")
    parser.add_argument("--max-cases", type=positive_int, help="最多运行多少个用例")
    parser.add_argument("--output-dir", type=Path,
                        help="在指定目录下新建独立运行目录，保留 IR、日志和 report.json")
    return parser.parse_args()


def find_lli(requested: str | None) -> tuple[list[str], int]:
    names = [requested] if requested else ["lli"] + [f"lli-{v}" for v in range(22, 13, -1)]
    fallback = None
    for name in names:
        executable = shutil.which(name)
        if not executable:
            continue
        probe = subprocess.run([executable, "--version"], capture_output=True,
                               text=True, timeout=10)
        match = re.search(r"LLVM version\s+(\d+)", probe.stdout + probe.stderr)
        if probe.returncode or not match:
            if requested:
                raise ValueError(f"无法识别 lli 版本：{executable}")
            continue
        version = int(match.group(1))
        if version < 14:
            continue
        # 当前 IR 使用 ptr；LLVM 14 需要显式开启 opaque pointers。
        command = [executable] + (["-opaque-pointers"] if version == 14 else [])
        # 不使用 lli 默认的代码生成优化等级来衡量学生的中端 pass。
        command.append("-O0")
        candidate = (command, version)
        if requested or version >= 15:
            return candidate
        fallback = candidate
    if fallback:
        return fallback
    raise FileNotFoundError("找不到可用的 LLVM 14+ lli，请通过 --lli 指定")


def find_llvm_as(requested: str | None, lli: str, version: int) -> list[str]:
    # 优先同一安装目录，再查 PATH；不能混用不同主版本的 IR 解析器。
    directories = [Path(lli).parent, Path(lli).resolve().parent]
    names = [requested] if requested else [
        str(directory / name)
        for directory in directories
        for name in (f"llvm-as-{version}", "llvm-as")
    ] + [f"llvm-as-{version}", "llvm-as"]
    for name in dict.fromkeys(names):
        executable = shutil.which(name)
        if not executable:
            continue
        probe = subprocess.run([executable, "--version"], capture_output=True,
                               text=True, timeout=10)
        match = re.search(r"LLVM version\s+(\d+)", probe.stdout + probe.stderr)
        if probe.returncode or not match or int(match.group(1)) != version:
            if requested:
                raise ValueError(f"llvm-as 必须是 LLVM {version}：{executable}")
            continue
        return [executable] + (["-opaque-pointers"] if version == 14 else [])
    raise FileNotFoundError(
        f"找不到 LLVM {version} 的 llvm-as，请安装配套工具或通过 --llvm-as 指定")


def collect_cases(args: argparse.Namespace) -> list[Path]:
    if not args.test_root.is_dir():
        raise FileNotFoundError(f"测试目录不存在：{args.test_root}")
    cases = sorted(p for p in args.test_root.rglob("*.sy") if p.is_file())
    if args.filter:
        cases = [p for p in cases if any(
            fragment in p.relative_to(args.test_root).as_posix() for fragment in args.filter
        )]
    if args.max_cases:
        cases = cases[:args.max_cases]
    if not cases:
        raise ValueError("没有找到符合条件的 .sy 用例")
    return cases


def inject_runtime(runtime: str, generated: str) -> str:
    """保留原始 IR；在供 lli 执行的副本中去掉与运行库重复的声明。"""
    symbols = set()
    for line in runtime.splitlines():
        match = FUNCTION_HEADER.match(line)
        if match:
            symbols.add(match.group(2))
    body = []
    for line in generated.splitlines(keepends=True):
        match = FUNCTION_HEADER.match(line)
        if match and match.group(1) == "declare" and match.group(2) in symbols:
            # 本项目 dumpDefaultDeclarations 输出单行声明，不猜测多行 IR 的边界。
            if ")" not in line:
                raise ValueError("运行库重复声明跨行，无法安全注入：" + line.strip())
            continue
        body.append(line)
    return runtime.rstrip() + "\n\n; ---- compiler generated IR ----\n" + "".join(body)


def log_tail(path: Path, limit: int = 2000) -> str:
    if not path.is_file():
        return ""
    with path.open("rb") as stream:
        stream.seek(0, os.SEEK_END)
        stream.seek(max(0, stream.tell() - limit))
        return stream.read().decode("utf-8", errors="replace").strip()


def has_lli_error(path: Path) -> bool:
    # 错误位置可能在很长的 IR 行之前；不能只搜索供终端显示的日志尾部。
    pattern = re.compile(rb"(?:error:|LLVM ERROR:|JIT session error:|Symbols not found:)")
    with path.open("rb") as stream:
        return any(pattern.search(line) for line in stream)


def execute(command: list[str], directory: Path, prefix: str,
            timeout: float, input_path: Path | None = None) -> tuple[int, float]:
    started = time.monotonic()
    with (input_path or Path(os.devnull)).open("rb") as stdin, \
            (directory / f"{prefix}.stdout").open("wb") as stdout, \
            (directory / f"{prefix}.stderr").open("wb") as stderr:
        result = subprocess.run(command, cwd=directory, stdin=stdin,
                                stdout=stdout, stderr=stderr, timeout=timeout)
    return result.returncode, time.monotonic() - started


def compare_output(stdout: bytes, returncode: int, reference: bytes) -> tuple[bool, str]:
    expected = reference.splitlines()
    if not expected or not re.fullmatch(rb"\d+", expected[-1].strip()):
        raise ValueError(".out 最后一行必须是 0..255 的十进制程序退出码")
    expected_code = int(expected[-1].strip())
    if not 0 <= expected_code <= 255:
        raise ValueError(".out 中的退出码超出 0..255")
    actual = stdout.splitlines()
    if actual == expected[:-1] and returncode == expected_code:
        return True, ""
    difference = unified_diff(
        [s.decode("utf-8", errors="replace") + "\n" for s in expected[:-1]],
        [s.decode("utf-8", errors="replace") + "\n" for s in actual],
        fromfile="expected stdout", tofile="actual stdout", n=2,
    )
    preview = ""
    for line in difference:
        preview += line[:2000]
        if len(preview) >= 2000:
            preview = preview[:2000] + "\n…（差异已截断）\n"
            break
    return False, f"退出码：期望 {expected_code}，实际 {returncode}\n" + preview.rstrip()


def run_case(args: argparse.Namespace, case: Path, work: Path,
             runtime: str, lli: list[str], llvm_as: list[str]) -> CaseResult:
    relative = case.relative_to(args.test_root)
    name = relative.as_posix()
    # 保留 .sy 后缀，避免 foo.sy 和 foo/bar.sy 的产物目录相互覆盖。
    directory = work / "cases" / relative
    compile_seconds = run_seconds = 0.0
    try:
        reference_path = case.with_suffix(".out")
        if not reference_path.is_file():
            return CaseResult(name, "MISSING", detail="缺少同名 .out 参考答案")
        reference = reference_path.read_bytes()
        compare_output(b"", 0, reference)  # 在编译前检查答案格式。
        directory.mkdir(parents=True)
        raw_ir = directory / "optimized.ll"
        run_ir = directory / "executable.ll"
        compile_command = [str(args.compiler), str(case), "-S", "-o", str(raw_ir),
                           f"-O{args.opt_level}"]
        run_command = lli + [str(run_ir)]
        verify_command = llvm_as + [str(raw_ir), "-o", os.devnull]
        (directory / "commands.json").write_text(json.dumps(
            {"compile": compile_command, "verify": verify_command, "run": run_command},
            ensure_ascii=False, indent=2
        ) + "\n", encoding="utf-8")
        try:
            code, compile_seconds = execute(compile_command, directory, "compile",
                                            args.compile_timeout)
        except subprocess.TimeoutExpired:
            return CaseResult(name, "COMPILE_TLE", args.compile_timeout,
                              detail=f"编译超过 {args.compile_timeout:g}s")
        if code != 0 or not raw_ir.is_file() or raw_ir.stat().st_size == 0:
            return CaseResult(name, "CE", compile_seconds,
                              detail=f"编译器退出码 {code}；未成功生成 IR\n" +
                              log_tail(directory / "compile.stderr"))
        # llvm-as 只解析并验证，不链接或执行；运行库函数的合法 declare 足够。
        # 必须先检查原文件，避免注入时删除重复声明而掩盖无效 IR。
        try:
            code, _ = execute(verify_command, directory, "verify", args.verify_timeout)
        except subprocess.TimeoutExpired:
            return CaseResult(name, "VERIFY_TLE", compile_seconds,
                              detail=f"原始 IR 验证超过 {args.verify_timeout:g}s")
        if code != 0:
            return CaseResult(name, "IR_ERROR", compile_seconds,
                              detail=f"原始 IR 验证失败，llvm-as 退出码 {code}\n" +
                              log_tail(directory / "verify.stderr"))
        run_ir.write_text(inject_runtime(runtime, raw_ir.read_text(encoding="utf-8")),
                          encoding="utf-8")
        input_path = case.with_suffix(".in")
        try:
            code, run_seconds = execute(run_command, directory, "lli", args.timeout,
                                        input_path if input_path.is_file() else None)
        except subprocess.TimeoutExpired:
            return CaseResult(name, "TLE", compile_seconds, args.timeout,
                              f"lli 执行超过 {args.timeout:g}s（含 JIT）")
        diagnostic = log_tail(directory / "lli.stderr")
        # SysY 的非零返回值是正常结果；负数才表示被信号终止。
        if code < 0:
            return CaseResult(name, "RE", compile_seconds, run_seconds,
                              f"lli 被信号 {-code} 终止\n{diagnostic}")
        if has_lli_error(directory / "lli.stderr"):
            return CaseResult(name, "IR_ERROR", compile_seconds, run_seconds, diagnostic)
        stdout = (directory / "lli.stdout").read_bytes()
        passed, detail = compare_output(stdout, code, reference)
        # 便于与 .out 直接 diff；定时运行库的 stderr 不进入判题输出。
        separator = b"\n" if stdout and not stdout.endswith(b"\n") else b""
        (directory / "actual.out").write_bytes(stdout + separator + f"{code}\n".encode())
        if not passed:
            (directory / "difference.txt").write_text(detail + "\n", encoding="utf-8")
        return CaseResult(name, "AC" if passed else "WA", compile_seconds, run_seconds, detail)
    except (OSError, UnicodeError, ValueError) as error:
        return CaseResult(name, "ERROR", compile_seconds, run_seconds, str(error))


def check_runtime(runtime: str, lli: list[str], work: Path) -> None:
    probe = work / "_runtime_check"
    probe.mkdir()
    program = probe / "probe.ll"
    program.write_text(inject_runtime(runtime, "define i32 @main() { ret i32 0 }\n"),
                       encoding="utf-8")
    code, _ = execute(lli + [str(program)], probe, "lli", 15)
    if code != 0:
        raise RuntimeError("运行库与 lli 不兼容，或当前主机无法执行该运行库：\n" +
                           log_tail(probe / "lli.stderr"))
    shutil.rmtree(probe)


def run_suite(args: argparse.Namespace, cases: list[Path], runtime: str,
              lli: list[str], llvm_as: list[str], work: Path) -> int:
    check_runtime(runtime, lli, work)
    started = time.monotonic()
    results = []
    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        futures = [pool.submit(run_case, args, case, work, runtime, lli, llvm_as)
                   for case in cases]
        try:
            for future in as_completed(futures):
                result = future.result()
                results.append(result)
                print(f"[{len(results):3}/{len(cases)}] [{result.status:11}] {result.case} "
                      f"compile={result.compile_seconds:.3f}s lli={result.run_seconds:.3f}s",
                      flush=True)
                if result.detail:
                    print(result.detail, flush=True)
        except KeyboardInterrupt:
            for future in futures:
                future.cancel()
            raise
    results.sort(key=lambda r: r.case)
    counts = Counter(r.status for r in results)
    elapsed = time.monotonic() - started
    print("\nSummary: " + ", ".join(f"{k}={v}" for k, v in sorted(counts.items())) +
          f"; total={len(results)}; elapsed={elapsed:.2f}s")
    for group in sorted({Path(r.case).parts[0] for r in results}):
        members = [r for r in results if Path(r.case).parts[0] == group]
        print(f"  {group}: {sum(r.status == 'AC' for r in members)}/{len(members)} AC")
    (work / "report.json").write_text(json.dumps({
        "compiler": str(args.compiler), "runtime": str(args.runtime),
        "lli_command": lli, "test_root": str(args.test_root), "opt_level": args.opt_level,
        "verify_command": llvm_as, "verify_timeout": args.verify_timeout,
        "jobs": args.jobs, "compile_timeout": args.compile_timeout, "timeout": args.timeout,
        "comparison": "exact lines and exit code", "elapsed_seconds": elapsed,
        "counts": dict(counts), "results": [asdict(r) for r in results],
    }, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    passed = counts.get("AC", 0) == len(cases)
    print("Overall: " + ("PASS" if passed else "FAIL"))
    return 0 if passed else 1


def main() -> int:
    args = parse_args()
    try:
        args.compiler = args.compiler.resolve()
        args.runtime = args.runtime.resolve()
        args.test_root = args.test_root.resolve()
        if not args.compiler.is_file() or not os.access(args.compiler, os.X_OK):
            raise FileNotFoundError(f"编译器不存在或不可执行：{args.compiler}；请先构建 task2")
        runtime = args.runtime.read_text(encoding="utf-8")
        if not runtime.strip():
            raise ValueError("运行库 .ll 为空")
        cases = collect_cases(args)
        lli, version = find_lli(args.lli)
        llvm_as = find_llvm_as(args.llvm_as, lli[0], version)
        print(f"[SETUP] compiler: {args.compiler}\n[SETUP] runtime: {args.runtime}\n"
              f"[SETUP] lli: {lli[0]} (LLVM {version})\n"
              f"[SETUP] llvm-as: {llvm_as[0]}\n[SETUP] cases: {len(cases)}, "
              f"-O{args.opt_level}, jobs: {args.jobs}", flush=True)
        if args.output_dir:
            args.output_dir.mkdir(parents=True, exist_ok=True)
            work = Path(tempfile.mkdtemp(prefix="run-", dir=args.output_dir.resolve()))
            print(f"[SETUP] 保留产物：{work}", flush=True)
            return run_suite(args, cases, runtime, lli, llvm_as, work)
        with tempfile.TemporaryDirectory(prefix="sysy-middle-end-") as temporary:
            return run_suite(args, cases, runtime, lli, llvm_as, Path(temporary))
    except (OSError, ValueError, RuntimeError, subprocess.SubprocessError) as error:
        print(f"[SETUP ERROR] {error}", file=sys.stderr)
        return 2
    except KeyboardInterrupt:
        print("\n测试已中断", file=sys.stderr)
        return 130


if __name__ == "__main__":
    raise SystemExit(main())
