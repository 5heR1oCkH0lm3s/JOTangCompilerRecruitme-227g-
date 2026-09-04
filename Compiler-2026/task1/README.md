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

本题保留完整 SysY2022 的数组、浮点数、函数、控制流和表达式。原 JOTang 编译器中的 `tensor` 类型与 `@` 运算符不是 SysY2022 标准内容，不属于必做范围。

## 资料

- 本地语言定义：`docs/SysY2022语言定义-V1-3.pdf`
- [SysY2022 运行时库](https://gitlab.eduxiji.net/csc1/nscscc/compiler2025/-/blob/main/SysY2022%E8%BF%90%E8%A1%8C%E6%97%B6%E5%BA%93-V1.pdf?ref_type=heads)
- [Flex 官方项目与手册](https://github.com/westes/flex)
- [GNU Bison 手册](https://www.gnu.org/software/bison/manual/)

## 目录

```text
task1/
├── CMakeLists.txt
├── main.cpp
├── README.md
├── docs/
│   └── SysY2022语言定义-V1-3.pdf
├── include/
│   ├── Frontend.hpp
│   └── lib/
│       └── AST.hpp
└── src/yacc/
    ├── sysy.l       # 等待实现
    └── sysy.y       # 等待实现
```

`AST.hpp` 提供 AST 节点、所有权接口和 Visitor 接口。原则上不应修改其公共接口；确有必要时，应在代码注释中说明原因。

`main.cpp` 和 `Frontend.hpp` 定义了统一入口及 `ASTRoot`。完成 `.l/.y` 后，无需手工生成或提交 `Flex.cpp`、`Bison.cpp`；CMake 会在构建目录中调用 Flex/Bison。

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

`sysy.l` 应包含生成的 `Bison.hpp`，并将扫描函数声明为：

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

完成 `sysy.l` 和 `sysy.y` 后，在本目录执行：

```bash
cmake -S . -B build
cmake --build build -j
./build/sysy_frontend path/to/test.sy
```

分析成功后，`ASTRoot` 持有完整 AST，程序输出顶层节点数量，例如：

```text
AST generated successfully: 2 top-level item(s)
```

下一轮将加入 ASTPrinter，以及针对 Token 和具体 AST 节点结构的自动测试。

## 必须完成

1. `src/yacc/sysy.l`；
2. `src/yacc/sysy.y`；
3. 自行设计的正例与反例测试；
4. 不提交手工修改的 Flex/Bison 生成文件。

允许使用 Agent，但提交者必须能够解释和维护最终代码。后续考核会重点检查规范一致性、边界输入、文法冲突、AST 结构和错误处理，而不是代码行数。
