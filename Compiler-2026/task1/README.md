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
│   ├── golden/       # 少量可直接阅读的 AST 标准答案
│   └── negative/     # 必须拒绝的非法程序
└── src/
    ├── lib/
    │   └── ASTPrinter.cpp
    └── yacc/
        ├── sysy.l       # 等待实现
        ├── sysy.y       # 等待实现
        ├── Flex.cpp     # 生成文件
        └── Bison.cpp    # 生成文件
```

`AST.hpp` 提供 AST 节点、所有权接口和 Visitor 接口。原则上不应修改其公共接口；确有必要时，应在代码注释中说明原因。

`main.cpp` 和 `Frontend.hpp` 定义了统一入口及 `ASTRoot`。`ASTPrinter` 负责将生成结果输出成稳定的 S-expression。完成 `.l/.y` 后，Flex/Bison 生成文件必须位于：

```text
src/yacc/Flex.cpp
src/yacc/Bison.cpp
include/yacc/Flex.hpp
include/yacc/Bison.hpp
```

CMake 不会调用 Flex/Bison。学生必须先使用后文命令手工生成这四个文件，再配置和构建项目。这些生成文件不提交到 Git。

每次修改 `sysy.l` 或 `sysy.y` 后，都需要重新执行对应的生成命令；CMake 只编译现有生成文件，不会检查并重新生成它们。

输出路径由后文的生成命令统一指定。请不要在 `.y` 中另写 `%output`、`%header`，也不要在 `.l` 中另写 `outfile`、`header-file`，避免其路径与命令行参数冲突。

## 与构建入口的接口约定

为了让提供的 `main.cpp`、CMake 和后续自动测试能够调用学生实现，`sysy.y` 应生成默认的 C++ 解析器 `yy::parser`，并启用强类型 Token 构造函数：

```bison
%language "c++"
%define api.token.constructor
%define api.value.type variant
```

在 Bison 的实现代码中声明扫描器入口：

```cpp
extern yy::parser::symbol_type yylex();
```

通过 `Frontend.hpp` 使用根节点，并在开始符号成功归约完整编译单元时接管结果：

```cpp
ASTRoot = std::unique_ptr<CompUnit>(...);
```

`sysy.l` 应包含 `yacc/Bison.hpp`，并将扫描函数声明为：

```cpp
#define YY_DECL yy::parser::symbol_type yylex(void)
```

扫描器必须启用行号并提供全局 `yyin`、`yylineno`。推荐使用 `%option noyywrap`，避免依赖平台特定的 `yywrap` 实现。

## 必做功能

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

## 不属于本题范围

- 符号表和作用域检查；
- 重复定义和未定义标识符检查；
- 类型检查和隐式类型转换；
- 数组初始化展开与补零；
- `break/continue` 是否位于循环内；
- Runtime 函数解析；
- IR、优化与汇编生成。

例如，循环外的 `break;` 可以通过本题语法分析；它是否语义合法由后续任务判断。

## 构建与运行

完成 `sysy.l` 和 `sysy.y` 后，必须在 `task1` 根目录依次手动生成文件：

```bash
mkdir -p include/yacc src/yacc
bison --defines=include/yacc/Bison.hpp \
      --output=src/yacc/Bison.cpp src/yacc/sysy.y
flex --header-file=include/yacc/Flex.hpp \
     --outfile=src/yacc/Flex.cpp src/yacc/sysy.l
```

确认四个生成文件位于规定目录后，再由学生手动配置并构建：

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
6. 运行 `tests/negative` 中的非法程序，确认词法器和语法分析器会拒绝它们。

测试脚本不会生成 Flex/Bison 文件，也不会调用 CMake。若可执行文件不在默认位置，可使用 `--compiler` 显式指定。

`.in/.out` 用于完整编译器运行测试，本任务只检查前端，因此不会读取它们。更多选项见 `tests/README.md`。

## 必须完成

1. `src/yacc/sysy.l`；
2. `src/yacc/sysy.y`；
3. 自行设计的正例与反例测试；
4. 能够通过 `test_frontend.py` 的批量测试；
5. 不提交 Flex/Bison 生成文件。

允许使用 Agent，但提交者必须能够解释和维护最终代码。后续考核会重点检查规范一致性、边界输入、文法冲突、AST 结构和错误处理，而不是代码行数。
