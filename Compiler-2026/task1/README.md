>实现报告见 ./_report/

# Task 1：使用 Flex/Bison 构建 SysY2022 前端

## 任务目标

从空白的 `sysy.l` 和 `sysy.y` 出发，实现完整前端流水线：

```text
SysY 源文件
  → Flex 词法分析
  → Token 流
  → Bison 语法分析
  → 归约时执行语义动作
  → ASTRoot
```

## 目录

```text
task1/
├── CMakeLists.txt
├── main.cpp
├── README.md
├── test_frontend.py
├── docs/
│   └── SysY2022语言定义-V1-3.pdf
├── include/
│   ├── ASTPrinter.hpp
│   ├── Frontend.hpp
│   ├── lib/
│   │   └── AST.hpp
│   └── yacc/         # 生成 Flex.hpp 与 Bison.hpp
├── tests/
│   ├── README.md
│   ├── ast.sha256
│   └── golden/       # 少量可直接阅读的 AST 标准答案
└── src/
    ├── lib/
    │   └── ASTPrinter.cpp
    └── yacc/
        ├── sysy.l       # 等待实现
        ├── sysy.y       # 等待实现
        ├── Flex.cpp     # 生成文件
        └── Bison.cpp    # 生成文件
```

本题支持模板 AST 和自建 AST 两条路线，统一按 [标准 AST 输出协议](docs/AST_FORMAT.md) 验收。自建 AST 须提供适配器生成规定的 S-expression；内部类和所有权设计不参与摘要比较。报告中说明接口调整与适配方法。

以下 ASTRoot、Flex/Bison 生成路径及手工构建说明适用于模板路线。自建路线可以调整入口与 CMake，但必须提供同样的命令行输入和标准输出，完成真实 AST 构造；详见协议。`AST.hpp` 为模板路线提供节点、所有权接口和 Visitor 接口。

`main.cpp` 和 `Frontend.hpp` 定义了统一入口及 `ASTRoot`。`ASTPrinter` 负责将生成结果输出成稳定的 S-expression。完成 `.l/.y` 后，Flex/Bison 生成文件必须位于：

```text
src/yacc/Flex.cpp
src/yacc/Bison.cpp
include/yacc/Flex.hpp
include/yacc/Bison.hpp
```

## 必做功能

> 注意：runtime中`putf`函数无需处理

### 1. 词法分析

使用 Flex 识别：

- SysY2022 关键字和标识符；
- 十进制、八进制、十六进制整数；
- 十进制和十六进制浮点数；
- 分隔符和运算符；
- 单行注释和多行注释；
- 文件结束。

词法器必须：

- 遵守最长匹配原则；
- 保存 Token 的正确语义值；
- 跟踪至少行号；
- 对非法字符给出错误，而不是静默忽略；
- 对未闭合的多行注释给出错误；
- 妥善处理数值转换异常和越界。

### 2. 语法分析

使用 Bison 将官方 EBNF 转写为可执行文法，覆盖：

- 编译单元、全局声明和函数定义；
- 常量与变量声明；
- 标量与多维数组；
- 标量初始化和嵌套初始化列表；
- 函数形参、实参和函数调用；
- 代码块与块内声明；
- 赋值、表达式、空语句；
- `if/else`、`while`；
- `break`、`continue`、`return`；
- 一元、乘除模、加减、关系、相等、逻辑与和逻辑或表达式。

必须正确体现官方规定的优先级、结合性和 `else` 就近匹配行为。不得通过忽略 Token 让非法程序通过分析。

### 3. AST 生成

不能只判断语法是否正确。Bison 每次归约时应通过语义动作逐步构造 AST，并在成功分析完整编译单元后设置唯一根节点。

应满足：

- 声明、形参、实参和块项保持源码顺序；
- 括号、逗号、分号等无独立语义的符号不必成为节点；
- 表达式结构唯一反映优先级和结合性；
- 数组维度、下标和嵌套初始化结构不丢失；
- 函数调用保留被调函数名及源码行号；
- 节点所有权清晰，不重复释放；
- 分析失败时尽量清理尚未被 AST 接管的语义值。

## 模板路线的构建与运行

完成 `sysy.l` 和 `sysy.y` 后，必须在 `task1` 根目录依次手动生成文件：

```bash
mkdir -p include/yacc src/yacc
bison --defines=include/yacc/Bison.hpp \
      --output=src/yacc/Bison.cpp src/yacc/sysy.y
flex --header-file=include/yacc/Flex.hpp \
     --outfile=src/yacc/Flex.cpp src/yacc/sysy.l
```

确认四个生成文件位于规定目录后，手动配置并构建：

```bash
cmake -S . -B build
cmake --build build -j
./build/sysy_frontend path/to/test.sy
```

分析成功后，`ASTRoot` 持有完整 AST，程序将 AST 输出为单行 S-expression，例如：

```text
(CompUnit (FuncDef int "main" (None) (Block ...)))
```

## 批量测试

`Compiler-2026/testcases26` 中包含 200 个 SysY 程序。运行：

```bash
python3 test_frontend.py
```

测试器会：

1. 使用学生已经构建好的 `build/sysy_frontend`；
2. 递归收集 `testcases26` 中的 `.sy`；
3. 为每个用例单独运行前端并检查退出码；
4. 检查 stdout 是否为规范化的 `(CompUnit ...)`；
5. 对完整 AST 输出计算 SHA-256，并与参考摘要比较；

## 必须完成

1. 模板路线：实现 `src/yacc/sysy.l` 和 `src/yacc/sysy.y`；自建路线：提交词法/语法分析、AST 构造及标准输出适配器；
2. 提供可复现的构建命令与报告，说明解析及适配流程；
3. 能够通过 `python3 test_frontend.py` 的 200 个用例和 `python3 test_frontend.py --examples` 的小型协议用例；
4. 无需提交最终的可执行文件

## 自建 AST 与诊断

两条路线均遵循 [AST_FORMAT.md](docs/AST_FORMAT.md) 的节点字段、空节点、表达式包装和函数名起始行号约定；摘要包含结尾唯一 LF。测试不会把不同结构自动判为等价，适配器需保留协议所需的语法分组信息。`--compiler` 可指定自建前端路径。

`tests/examples` 提供完整可读的源码/AST 对。使用 `--output-dir build/ast-debug` 保存完整实际 AST、stderr，以及可用的 expected/diff；终端显示首个不同字节的邻近内容。两组验收均可使用该选项。`--no-golden` 不属于正式通过结果。
