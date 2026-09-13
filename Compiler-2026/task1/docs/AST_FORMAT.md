# Task1 标准 AST 输出协议 v1

本协议是统一验收的序列化格式，不限制学生内部 AST 的类、容器或所有权设计。自建 AST 必须通过适配器输出本格式；可转换成题目 AST 后调用 `ASTPrinter`，也可直接实现序列化。两条路线使用相同用例和摘要，不以内部类型或指针布局评分。

模板路线使用 `ASTRoot`、Flex/Bison 与原构建入口。自建路线可调整 `main.cpp`、头文件及 CMake，不强制保留 `ASTRoot` 或生成文件布局，但须提供 `build/sysy_frontend`（或通过测试器 `--compiler` 指定），接受一个源文件路径参数。保留官方打印器、测试器、用例和摘要作为格式依据，不修改这些文件绕过验收。自建路线同样完成词法、语法与实际 AST 构造，在报告中解释其设计与适配方式。

## 字节格式

成功时退出码为 0；stdout 只包含一行 UTF-8 S-expression，末尾恰好一个 LF（`\n`），无 BOM、CR、首尾空格或日志。错误时非零退出，诊断写 stderr。摘要对完整 stdout 原始字节计算 SHA-256，不自动忽略空格或换行。

节点格式为 `(Tag 字段...)`，相邻字段之间恰好一个 ASCII 空格；空节点为 `(Tag)`。标识符用双引号包围，大小写保持源码，转义规则与 C++ `std::quoted` 一致。类型为 `int`、`float`、`void`。整数输出解析后的十进制值，浮点输出 binary32 数值对应的 C++ `std::hexfloat` 小写形式（例如 `1.5` 为 `0x1.8p+0`），不保留原字面量进制和拼写。可复用打印器避免自行处理浮点格式差异。

## 节点与字段顺序

表中尖括号、星号和方括号是说明记号，不出现在实际输出中；`*` 表示按源码顺序重复，`[字段]` 表示该字段可省略。

| 节点 | 格式 |
|---|---|
| 编译单元 | `(CompUnit <声明或函数>*)` |
| 声明 | `(ConstDecl <类型> <ConstDefList>)` / `(VarDecl <类型> <VarDefList>)` |
| 定义列表 | `(ConstDefList <ConstDef>*)` / `(VarDefList <VarDef>*)` |
| 定义 | `(ConstDef "名" <维度或None> <InitVal>)` / `(VarDef "名" <维度或None> <InitVal或None>)` |
| 数组维度/下标 | `(ArrayList <AddExp>*)`，保留源码维度和下标顺序 |
| 初始化 | `(InitVal [<AddExp或InitValList>])`，列表为 `(InitValList <InitVal>*)` |
| 函数定义 | `(FuncDef <类型> "名" <FuncParamList或None> <Block>)` |
| 形参列表 | `(FuncParamList <FuncParam>*)` |
| 形参 | `(FuncParam <类型> "名" <维度标记> <ArrayList或None>)` |
| 块 | `(Block [<BlockItemList>])`，块项为 `(BlockItemList <声明或语句>*)` |
| 赋值 | `(AssignStmt <LVal> <AddExp>)` |
| 表达式语句 | `(ExpStmt [<AddExp>])` |
| 分支 | `(IfStmt <LOrExp> <then语句> <else语句或None>)` |
| 循环 | `(WhileStmt <LOrExp> <语句>)` |
| 跳转 | `(BreakStmt)` / `(ContinueStmt)` / `(ReturnStmt [<AddExp>])` |
| 左值 | `(LVal "名" <ArrayList或None>)` |
| 调用 | `(FuncCall "函数名" <行号> <FuncRParamList或None>)` |
| 实参列表 | `(FuncRParamList <AddExp>*)` |
| 常量 | `(Int <十进制值>)` / `(Float <十六进制浮点值>)` |

形参维度标记：标量使用 `scalar-or-explicit-first-dimension` 且后接 `(None)`；数组形参首个 `[]` 使用 `empty-first-dimension`，后续显式维度放入 ArrayList，没有后续维度则为 `(None)`。标记中的 explicit 字样是格式兼容名称，不扩大 SysY2022 文法范围。

## 空值与行号

缺少维度、初始化、形参、实参或 else 分支时，对应固定字段输出 `(None)`。以下是特例，不能用 `(None)` 替代：空块为 `(Block)`，空语句为 `(ExpStmt)`，无值返回为 `(ReturnStmt)`，空初始化花括号 `{}` 为 `(InitVal)`。嵌套花括号保留 InitVal/InitValList 层级，不展开、不补零。

FuncCall 行号从 1 起，取词法器识别到函数名起始位置的源码行；跨行参数、括号和注释不改变这个值。所有调用都记录，包括 `starttime`/`stoptime`，序列化时保留源码函数名。

## 表达式：层级、列表与括号

每层格式均为 `(层名 (Ops 运算符*) 操作数*)`。即使没有运算符，也保留 `(Ops)` 与该层包装。

普通 Exp/ConstExp 从 AddExp 开始：`AddExp → MulExp → UnaryExp → 常量/LVal/FuncCall/括号内AddExp`。Cond 在其外增加 `LOrExp → LAndExp → EqExp → RelExp`。不得常量折叠、重排运算或去掉单操作数包装层。

同层二元链按源码顺序平铺，N 个操作数对应 N−1 个运算符，按左结合解释。例如 `a-b-c` 的 AddExp 中是 `(Ops - -)` 和三个 MulExp，而不是嵌套两个 AddExp。一元链从外到内保存操作符，只有一个基本操作数，例如 `-+a` 为 `(UnaryExp (Ops - +) (LVal "a" (None)))`。

括号本身不输出节点，但括号内的 AddExp 作为外层 UnaryExp 的操作数保留。因此 `(a+b)*c` 不能被适配器误写成 `a+b*c`，`a+(b+c)` 也不能随意拍平成 `a+b+c`。自建 AST 如通常会丢弃冗余括号，须在解析时记录源语法分组信息，以便适配本协议。

例如 `return 1+2*3;` 的返回节点是：

```text
(ReturnStmt (AddExp (Ops +) (MulExp (Ops) (UnaryExp (Ops) (Int 1))) (MulExp (Ops *) (UnaryExp (Ops) (Int 2)) (UnaryExp (Ops) (Int 3)))))
```

## 可读样例与诊断

`tests/examples/` 提供成对的 `.sy` / `.ast` 及 `ast.sha256`，覆盖空值、优先级、一元链、括号、数组/初始化、形参、多行调用、数值和控制流。它们是正式协议用例；原 `testcases26` 的 200 个摘要保持不变。

在 task1 目录分别运行两组验收：

```bash
python3 test_frontend.py --examples
python3 test_frontend.py
```

使用 `--output-dir build/ast-debug` 会创建独立运行目录，保存完整实际输出、stderr；WA 且有可读 golden 时还保存 expected 和完整 diff。终端给出首个不同字节及邻近文本，避免大型单行 diff 只显示相同前缀。`--no-golden` 仅用于冒烟排查，不算摘要验收通过。
