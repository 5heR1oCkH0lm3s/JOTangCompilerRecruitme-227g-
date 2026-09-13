# Task3：RISC-V 调用约定与寄存器分配

本任务独立于 Task1/Task2，可直接使用给出的 C 与汇编程序。基础部分无需实现编译器后端；QEMU 用来执行交叉编译得到的 Linux 用户态程序，无需启动完整虚拟机。

## 基础任务与验收

1. 完成下面的环境检查，记录工具版本、三组输出与退出码。
2. 阅读 `examples/call.c` 和 `examples/call.S`，在 `task3.md` 中画出 `main → add_with_bias → add_pair` 调用链和 `add_with_bias` 的栈帧。
3. 对照反汇编解释：两个参数和返回值在哪些寄存器中；为什么保存 `ra`、`s0`；谁负责保存 caller-saved/callee-saved 寄存器；为什么栈帧分配 16 字节；恢复顺序和 `ret` 的作用。区分伪指令与反汇编结果，指令地址和编码不要求一致。
4. 自选一组不同于脚本的、小范围有符号整数输入，先手算 `left + right + 5`，再运行验证并记录结果。说明如果把 `s0` 换成 caller-saved 临时寄存器，跨调用保存值需要什么条件。
5. 解释虚拟寄存器、物理寄存器、活跃区间和 spill；任选线性扫描或图着色，用一个小例子说明寄存器不足时如何分配。

环境输出正确与报告解释正确共同构成基础验收；只贴脚本的 PASS 不算完成。无需改动示例，禁止把预期结果写死替代实际运行。

## 环境与可复现命令

统一目标为 Linux ELF、RV64GC、LP64D ABI，使用静态链接。建议使用 Linux；Windows 可在 WSL2 Linux 中完成，macOS 可使用 Linux 虚拟机。不同主机不比较 QEMU 运行速度。

Debian/Ubuntu 的安装命令（由学生自行执行）：

```bash
sudo apt update
sudo apt install gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu libc6-dev-riscv64-cross qemu-user
```

在 `Compiler-2026/task3` 中运行：

```bash
bash check_environment.sh
```

脚本使用临时目录并自动清理，默认需要上述工具位于 PATH。可通过 `TASK3_CC`、`TASK3_QEMU`、`TASK3_OBJDUMP` 指定单个可执行文件路径。三组结果应为：

```text
PASS: 17 20 -> 42 (exit=0)
PASS: -5 8 -> 8 (exit=0)
PASS: 0 0 -> 5 (exit=0)
```

手动构建、查看汇编并执行独立输入的等价命令：

```bash
mkdir -p build
riscv64-linux-gnu-gcc -march=rv64gc -mabi=lp64d -O0 -g -static examples/call.c examples/call.S -o build/call
riscv64-linux-gnu-objdump -d --disassemble=add_with_bias build/call
qemu-riscv64 build/call 17 20
printf 'exit=%s\n' "$?"
```

预期 stdout 为 `42`，退出码为 0。使用小范围十进制整数即可，本示例不考察输入校验或整数溢出。若提示缺少 `crt*.o` 或 `-lc`，检查交叉 libc 开发包；不要改用裸机 `riscv64-unknown-elf` 工具链。环境仍受限时，提交实际错误及排查过程，标注实验未完成，不影响其他任务独立验收。

## 进阶与拓展

进阶：阅读总题面提供的线性扫描、图着色和 LLVM Greedy 资料，比较三者的分配策略、spill 选择与编译开销。用自选例子解释差异，不要求实现三种分配器。

拓展：使用 QEMU 的 GDB 接口观察调用前后 `sp`、`ra`、`s0`、`a0`、`a1`，或生成同一段 C 代码在不同优化等级下的汇编并解释差异。任选一项深入完成即可。

## 提交

提交 `task3.md`：环境版本、完整命令、实际输出及退出码、栈帧图、调用约定分析、独立输入实验、完成的进阶项和引用来源。源码有修改时同时提交源码与修改原因，不提交二进制或 build 目录。AI 使用方式和借鉴来源按 [作答与提交约定](../SUBMISSION.md) 记录。

## 官方参考

- [QEMU Linux 用户态模拟](https://www.qemu.org/docs/master/user/main.html)
- [RISC-V psABI：寄存器和调用约定](https://riscv-non-isa.github.io/riscv-elf-psabi-doc/)
- [GCC RISC-V 目标与 ABI 选项](https://gcc.gnu.org/onlinedocs/gcc/RISC-V-Options.html)
- [GNU RISC-V 工具链构建说明](https://github.com/riscv-collab/riscv-gnu-toolchain)
