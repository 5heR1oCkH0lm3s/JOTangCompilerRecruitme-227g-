#!/usr/bin/env bash
set -euo pipefail
TASK3_ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
TASK3_CC="${TASK3_CC:-riscv64-linux-gnu-gcc}"
TASK3_QEMU="${TASK3_QEMU:-qemu-riscv64}"
TASK3_OBJDUMP="${TASK3_OBJDUMP:-riscv64-linux-gnu-objdump}"
for tool in "$TASK3_CC" "$TASK3_QEMU" "$TASK3_OBJDUMP"; do
    command -v "$tool" >/dev/null || { echo "缺少工具：$tool" >&2; exit 2; }
done
TASK3_WORK="$(mktemp -d)"
trap 'rm -rf -- "$TASK3_WORK"' EXIT
"$TASK3_CC" --version
"$TASK3_QEMU" --version
"$TASK3_CC" -march=rv64gc -mabi=lp64d -O0 -g -static \
    "$TASK3_ROOT/examples/call.c" "$TASK3_ROOT/examples/call.S" -o "$TASK3_WORK/call"
for row in '17 20 42' '-5 8 8' '0 0 5'; do
    read -r left right expected <<< "$row"
    actual="$("$TASK3_QEMU" "$TASK3_WORK/call" "$left" "$right")"
    if [[ "$actual" != "$expected" ]]; then
        echo "FAIL: $left $right; expected=$expected actual=$actual" >&2
        exit 1
    fi
    echo "PASS: $left $right -> $actual (exit=0)"
done
"$TASK3_OBJDUMP" -d --disassemble=add_with_bias "$TASK3_WORK/call"
echo '环境检查通过；还需在报告中解释调用约定并完成独立输入实验。'
