#!/usr/bin/env python3
"""批量验证 SysY 前端能否解析 testcases26 并生成规范化 AST。"""

from __future__ import annotations

import argparse
import hashlib
import os
import subprocess
import sys
import time
from concurrent.futures import ThreadPoolExecutor, as_completed
from dataclasses import dataclass
from difflib import unified_diff
from pathlib import Path


TASK_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_TEST_ROOT = TASK_ROOT.parent / "testcases26"
DEFAULT_BUILD_DIR = TASK_ROOT / "build"
DEFAULT_MANIFEST = TASK_ROOT / "tests" / "ast.sha256"
DEFAULT_NEGATIVE_ROOT = TASK_ROOT / "tests" / "negative"


@dataclass(frozen=True)
class CaseResult:
    case: Path
    status: str
    elapsed: float
    detail: str = ""


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="构建 task1，并批量校验 testcases26 中每个 .sy 的 AST 输出。"
    )
    parser.add_argument("--test-root", type=Path, default=DEFAULT_TEST_ROOT)
    parser.add_argument("--build-dir", type=Path, default=DEFAULT_BUILD_DIR)
    parser.add_argument("--compiler", type=Path)
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--skip-build", action="store_true")
    parser.add_argument("--no-golden", action="store_true",
                        help="只检查解析成功和 AST 基本格式，不比较参考摘要")
    parser.add_argument("--no-negative", action="store_true",
                        help="不运行 task1/tests/negative 中的拒绝测试")
    parser.add_argument("--filter", action="append", default=[],
                        help="只运行相对路径中包含该字符串的用例，可重复")
    parser.add_argument("--max-cases", type=int)
    parser.add_argument("--jobs", type=int,
                        default=max(1, min(os.cpu_count() or 1, 8)))
    parser.add_argument("--timeout", type=float, default=10.0)
    parser.add_argument("--build-timeout", type=float, default=180.0)
    return parser.parse_args()


def run_setup(command: list[str], timeout: float) -> None:
    result = subprocess.run(
        command,
        cwd=TASK_ROOT,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        timeout=timeout,
    )
    if result.returncode != 0:
        print(result.stdout, end="", file=sys.stderr)
        raise RuntimeError(
            f"命令失败（退出码 {result.returncode}）：{' '.join(command)}"
        )


def build_frontend(build_dir: Path, timeout: float) -> Path:
    print("[SETUP] configuring task1 ...")
    run_setup(
        ["cmake", "-S", str(TASK_ROOT), "-B", str(build_dir)], timeout
    )
    print("[SETUP] generating Flex/Bison sources and building ...")
    run_setup(
        ["cmake", "--build", str(build_dir), "-j", str(max(1, min(os.cpu_count() or 1, 8)))],
        timeout,
    )

    generated_files = [
        TASK_ROOT / "src/yacc/Flex.cpp",
        TASK_ROOT / "src/yacc/Bison.cpp",
        TASK_ROOT / "include/yacc/Flex.hpp",
        TASK_ROOT / "include/yacc/Bison.hpp",
    ]
    missing = [str(path) for path in generated_files if not path.is_file()]
    if missing:
        raise RuntimeError("构建后缺少生成文件：\n" + "\n".join(missing))

    compiler = build_dir / "sysy_frontend"
    if not compiler.is_file():
        raise RuntimeError(f"未找到前端可执行文件：{compiler}")
    return compiler


def load_manifest(path: Path) -> dict[str, str]:
    expected: dict[str, str] = {}
    if not path.is_file():
        return expected
    for line_number, raw_line in enumerate(
        path.read_text(encoding="utf-8").splitlines(), start=1
    ):
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue
        parts = line.split(maxsplit=1)
        if len(parts) != 2 or len(parts[0]) != 64:
            raise ValueError(f"摘要文件第 {line_number} 行格式错误：{raw_line}")
        digest, relative_path = parts
        expected[relative_path] = digest.lower()
    return expected


def collect_cases(test_root: Path, filters: list[str], limit: int | None) -> list[Path]:
    if not test_root.is_dir():
        raise FileNotFoundError(f"测试目录不存在：{test_root}")
    cases = sorted(path for path in test_root.rglob("*.sy") if path.is_file())
    if filters:
        cases = [
            case for case in cases
            if any(text in case.relative_to(test_root).as_posix() for text in filters)
        ]
    if limit is not None:
        if limit < 0:
            raise ValueError("--max-cases 不能为负数")
        cases = cases[:limit]
    return cases


def looks_like_ast(output: bytes) -> bool:
    try:
        text = output.decode("utf-8")
    except UnicodeDecodeError:
        return False
    stripped = text.strip()
    return (
        stripped.startswith("(CompUnit")
        and stripped.endswith(")")
        and "\n" not in stripped
    )


def run_case(
    compiler: Path,
    case: Path,
    test_root: Path,
    expected: dict[str, str],
    timeout: float,
    compare_golden: bool,
) -> CaseResult:
    started = time.monotonic()
    try:
        result = subprocess.run(
            [str(compiler), str(case)],
            cwd=TASK_ROOT,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            timeout=timeout,
        )
    except subprocess.TimeoutExpired:
        return CaseResult(case, "TLE", time.monotonic() - started,
                          f"超过 {timeout:g}s")

    elapsed = time.monotonic() - started
    if result.returncode != 0:
        detail = result.stderr.decode("utf-8", errors="replace").strip()
        return CaseResult(case, "PE", elapsed, detail[-1000:])
    if not looks_like_ast(result.stdout):
        preview = result.stdout.decode("utf-8", errors="replace")[:500]
        return CaseResult(case, "BAD", elapsed,
                          "stdout 不是单行 CompUnit S-expression：" + preview)

    relative = case.relative_to(test_root).as_posix()
    if not compare_golden:
        return CaseResult(case, "OK", elapsed)
    reference = expected.get(relative)
    if reference is None:
        return CaseResult(case, "MISS", elapsed, "摘要文件中没有该用例")
    actual = hashlib.sha256(result.stdout).hexdigest()
    if actual != reference:
        actual_text = result.stdout.decode("utf-8", errors="replace")
        readable_golden = (
            TASK_ROOT / "tests/golden" / Path(relative)
        ).with_suffix(".ast")
        if readable_golden.is_file():
            expected_text = readable_golden.read_text(encoding="utf-8")
            difference = "".join(unified_diff(
                expected_text.splitlines(keepends=True),
                actual_text.splitlines(keepends=True),
                fromfile="expected.ast",
                tofile="actual.ast",
            ))
            comparison_detail = "\n" + difference[:4000]
        else:
            comparison_detail = f"\nAST prefix={actual_text[:500]}"
        return CaseResult(
            case,
            "WA",
            elapsed,
            f"expected sha256={reference}\nactual   sha256={actual}"
            f"{comparison_detail}",
        )
    return CaseResult(case, "AC", elapsed)


def run_negative_case(compiler: Path, case: Path, timeout: float) -> CaseResult:
    started = time.monotonic()
    try:
        result = subprocess.run(
            [str(compiler), str(case)],
            cwd=TASK_ROOT,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            timeout=timeout,
        )
    except subprocess.TimeoutExpired:
        return CaseResult(case, "TLE", time.monotonic() - started,
                          f"负例分析超过 {timeout:g}s")

    elapsed = time.monotonic() - started
    if result.returncode != 0:
        return CaseResult(case, "AC", elapsed)
    preview = result.stdout.decode("utf-8", errors="replace")[:500]
    return CaseResult(
        case,
        "NA",
        elapsed,
        "非法程序被前端接受" + (f"：{preview}" if preview else ""),
    )


def display_results(
    title: str,
    results: list[CaseResult],
    relative_root: Path,
) -> list[CaseResult]:
    print(f"\n{title}")
    counts: dict[str, int] = {}
    for index, result in enumerate(results, start=1):
        counts[result.status] = counts.get(result.status, 0) + 1
        relative = result.case.relative_to(relative_root).as_posix()
        print(
            f"[{result.status:4}] {relative:<55} "
            f"{result.elapsed:7.3f}s [{index}/{len(results)}]"
        )
        if result.detail and result.status not in {"AC", "OK"}:
            for line in result.detail.splitlines():
                print(f"       {line}")
    summary = ", ".join(f"{name}={count}" for name, count in sorted(counts.items()))
    print(f"Summary: {summary}; total={len(results)}")
    return [result for result in results if result.status not in {"AC", "OK"}]


def main() -> int:
    args = parse_args()
    test_root = args.test_root.resolve()
    build_dir = args.build_dir.resolve()

    try:
        if args.compiler:
            compiler = args.compiler.resolve()
            if not args.skip_build:
                build_frontend(build_dir, args.build_timeout)
        elif args.skip_build:
            compiler = build_dir / "sysy_frontend"
        else:
            compiler = build_frontend(build_dir, args.build_timeout)

        if not compiler.is_file():
            raise FileNotFoundError(f"前端可执行文件不存在：{compiler}")

        expected = {} if args.no_golden else load_manifest(args.manifest.resolve())
        if not args.no_golden and not expected:
            raise RuntimeError(
                f"参考 AST 摘要不存在或为空：{args.manifest.resolve()}；"
                "若只想做解析冒烟测试，请使用 --no-golden"
            )
        cases = collect_cases(test_root, args.filter, args.max_cases)
    except (OSError, RuntimeError, ValueError, subprocess.TimeoutExpired) as error:
        print(f"[SETUP ERROR] {error}", file=sys.stderr)
        return 2

    if not cases:
        print("[SETUP ERROR] 没有找到符合条件的 .sy 用例", file=sys.stderr)
        return 2

    print(f"[SETUP] compiler: {compiler}")
    print(f"[SETUP] cases: {len(cases)}, jobs: {max(1, args.jobs)}")

    results: list[CaseResult] = []
    with ThreadPoolExecutor(max_workers=max(1, args.jobs)) as pool:
        futures = {
            pool.submit(
                run_case,
                compiler,
                case,
                test_root,
                expected,
                args.timeout,
                not args.no_golden,
            ): case
            for case in cases
        }
        for future in as_completed(futures):
            results.append(future.result())

    results.sort(key=lambda item: item.case.relative_to(test_root).as_posix())
    failures = display_results("Positive AST cases", results, test_root)

    if not args.no_negative:
        negative_cases = sorted(DEFAULT_NEGATIVE_ROOT.glob("*.sy"))
        negative_results: list[CaseResult] = []
        with ThreadPoolExecutor(max_workers=max(1, args.jobs)) as pool:
            futures = {
                pool.submit(run_negative_case, compiler, case, args.timeout): case
                for case in negative_cases
            }
            for future in as_completed(futures):
                negative_results.append(future.result())
        negative_results.sort(key=lambda item: item.case.name)
        failures.extend(
            display_results("Negative rejection cases", negative_results,
                            DEFAULT_NEGATIVE_ROOT)
        )

    print(f"\nOverall: {'PASS' if not failures else 'FAIL'}")
    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
