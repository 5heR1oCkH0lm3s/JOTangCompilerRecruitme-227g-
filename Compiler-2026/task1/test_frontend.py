from __future__ import annotations

import argparse
import hashlib
import os
import subprocess
import sys
import tempfile
import time
from concurrent.futures import ThreadPoolExecutor, as_completed
from dataclasses import dataclass
from difflib import unified_diff
from pathlib import Path


TASK_ROOT = Path(__file__).resolve().parent
DEFAULT_TEST_ROOT = TASK_ROOT.parent / "testcases26"
DEFAULT_BUILD_DIR = TASK_ROOT / "build"
DEFAULT_MANIFEST = TASK_ROOT / "tests" / "ast.sha256"


@dataclass(frozen=True)
class CaseResult:
    case: Path
    status: str
    elapsed: float
    detail: str = ""


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="批量校验 testcases26 中每个 .sy 的 AST 输出。"
    )
    parser.add_argument("--examples", action="store_true", help="运行小型标准输出协议用例")
    parser.add_argument("--test-root", type=Path)
    parser.add_argument("--compiler", type=Path,
                        default=DEFAULT_BUILD_DIR / "sysy_frontend")
    parser.add_argument("--manifest", type=Path)
    parser.add_argument("--golden-root", type=Path, help="可读 .ast 的根目录，与用例相对路径对应")
    parser.add_argument("--output-dir", type=Path, help="新建独立运行目录，保存完整输出与差异")
    parser.add_argument("--no-golden", action="store_true",
                        help="只检查解析成功和 AST 基本格式，不比较参考摘要")
    parser.add_argument("--filter", action="append", default=[],
                        help="只运行相对路径中包含该字符串的用例，可重复")
    parser.add_argument("--max-cases", type=int)
    parser.add_argument("--jobs", type=int,
                        default=max(1, min(os.cpu_count() or 1, 8)))
    parser.add_argument("--timeout", type=float, default=10.0)
    return parser.parse_args()


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
    golden_root: Path,
    output_root: Path | None,
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
    relative = case.relative_to(test_root).as_posix()
    artifact = output_root / relative if output_root else None
    if artifact:
        artifact.parent.mkdir(parents=True, exist_ok=True)
        Path(str(artifact) + ".actual.ast").write_bytes(result.stdout)
        Path(str(artifact) + ".stderr").write_bytes(result.stderr)
    if result.returncode != 0:
        detail = result.stderr.decode("utf-8", errors="replace").strip()
        return CaseResult(case, "PE", elapsed, detail[-1000:])
    if not looks_like_ast(result.stdout):
        preview = result.stdout.decode("utf-8", errors="replace")[:500]
        return CaseResult(case, "BAD", elapsed,
                          "stdout 不是单行 CompUnit S-expression：" + preview)

    if not compare_golden:
        return CaseResult(case, "OK", elapsed)
    reference = expected.get(relative)
    if reference is None:
        return CaseResult(case, "MISS", elapsed, "摘要文件中没有该用例")
    actual = hashlib.sha256(result.stdout).hexdigest()
    if actual != reference:
        actual_text = result.stdout.decode("utf-8", errors="replace")
        readable_golden = (
            golden_root / Path(relative)
        ).with_suffix(".ast")
        if readable_golden.is_file():
            expected_bytes = readable_golden.read_bytes()
            expected_text = expected_bytes.decode("utf-8")
            difference = "".join(unified_diff(
                expected_text.splitlines(keepends=True),
                actual_text.splitlines(keepends=True),
                fromfile="expected.ast",
                tofile="actual.ast",
            ))
            offset = next((i for i, (left, right) in enumerate(zip(expected_bytes, result.stdout))
                           if left != right), min(len(expected_bytes), len(result.stdout)))
            start = max(0, offset - 80)
            comparison_detail = (
                f"\n首个不同字节（从 0 起）={offset}；"
                f"期望长度={len(expected_bytes)}，实际长度={len(result.stdout)}"
                f"\nexpected context={expected_bytes[start:offset + 160]!r}"
                f"\nactual   context={result.stdout[start:offset + 160]!r}"
            )
            if artifact:
                Path(str(artifact) + ".expected.ast").write_bytes(expected_bytes)
                Path(str(artifact) + ".diff").write_text(difference, encoding="utf-8")
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
    examples = TASK_ROOT / "tests/examples"
    test_root = (args.test_root or (examples if args.examples else DEFAULT_TEST_ROOT)).resolve()
    manifest = args.manifest or (examples / "ast.sha256" if args.examples else DEFAULT_MANIFEST)
    golden_root = (args.golden_root or (examples if args.examples else TASK_ROOT / "tests/golden")).resolve()
    compiler = args.compiler.resolve()

    try:
        if not compiler.is_file():
            raise FileNotFoundError(
                f"前端可执行文件不存在：{compiler}\n"
                "请先按 README.md 手工生成 Flex/Bison 文件并完成 CMake 构建。"
            )

        expected = {} if args.no_golden else load_manifest(manifest.resolve())
        if not args.no_golden and not expected:
            raise RuntimeError(
                f"参考 AST 摘要不存在或为空：{manifest.resolve()}；"
                "若只想做解析冒烟测试，请使用 --no-golden"
            )
        cases = collect_cases(test_root, args.filter, args.max_cases)
        output_root = None
        if args.output_dir:
            args.output_dir.mkdir(parents=True, exist_ok=True)
            output_root = Path(tempfile.mkdtemp(prefix="run-", dir=args.output_dir.resolve()))
    except (OSError, RuntimeError, ValueError) as error:
        print(f"[SETUP ERROR] {error}", file=sys.stderr)
        return 2

    if not cases:
        print("[SETUP ERROR] 没有找到符合条件的 .sy 用例", file=sys.stderr)
        return 2

    print(f"[SETUP] compiler: {compiler}")
    print(f"[SETUP] cases: {len(cases)}, jobs: {max(1, args.jobs)}")
    if output_root:
        print(f"[SETUP] 完整输出：{output_root}")

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
                golden_root,
                output_root,
            ): case
            for case in cases
        }
        for future in as_completed(futures):
            results.append(future.result())

    results.sort(key=lambda item: item.case.relative_to(test_root).as_posix())
    failures = display_results("AST cases", results, test_root)

    print(f"\nOverall: {'PASS' if not failures else 'FAIL'}")
    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
