/*
 * Task1 语法分析器（Bison C++ 骨架）—— SysY2022，归约时直接构造 AST。
 * 生成：bison --defines=include/yacc/Bison.hpp --output=src/yacc/Bison.cpp src/yacc/sysy.y
 *
 * 所有权约定：每个非终结符的语义值都是 std::unique_ptr。
 *   - 建节点：new XXX(...) 把裸指针交给节点的构造函数接管；
 *   - 转移：std::move($n) 整体搬家；取裸指针用 $n.release()。
 *   所以解析失败时，栈上还没交出去的子树会被 unique_ptr 自动释放，不需要 %destructor。
 */
%language "c++"
%require "3.2"

%define api.token.constructor    /* 扫描器返回 make_XXX(值) 形式的 token */
%define api.value.type variant   /* 语义值带类型信息，才能放 unique_ptr */
%define parse.error verbose      /* 语法错误信息更具体 */

/* %code requires 会写进 Bison.hpp：%token/%nterm 用到的类型必须在这里可见 */
%code requires {
    #include "lib/AST.hpp"

    #include <memory>
    #include <string>

    /* 标识符的语义值：名字 + 词法器看到它时的行号（FuncCall 要记函数名所在行） */
    struct LocatedIdentifier {
        std::string text;
        int line;
    };
}

/* %code 只写进 Bison.cpp，位置在 #include "Bison.hpp" 之后 */
%code {
    #include "Frontend.hpp"   /* extern std::unique_ptr<CompUnit> ASTRoot; */

    #include <iostream>

    extern yy::parser::symbol_type yylex();   /* token 由 Flex 提供 */
    extern int yylineno;                      /* 行号由 Flex 维护 */

    void yy::parser::error(const std::string& msg) {
        std::cerr << "语法错误（第 " << yylineno << " 行）：" << msg << '\n';
    }
}

/* 终结符：尖括号里是它携带的值的类型 */
%token <int> INT_CONST
%token <float> FLOAT_CONST
%token <LocatedIdentifier> IDENT

%token CONST INT FLOAT VOID
%token RETURN IF ELSE WHILE BREAK CONTINUE
%token LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET SEMICOLON COMMA
%token OR AND NOT EQ NOTEQ LESS GREAT LESSEQ GREATEQ
%token ADD SUB MUL DIV MOD ASSIGN

/* 非终结符：只要动作里用到 $$ / $n，就必须在这里登记类型 */
%nterm <ASTType> Type
%nterm <std::unique_ptr<CompUnit>> CompUnit
%nterm <std::unique_ptr<BaseAST>> Decl ConstDecl VarDecl BlockItem Stmt
%nterm <std::unique_ptr<ConstDefList>> ConstDefList
%nterm <std::unique_ptr<ConstDef>> ConstDef
%nterm <std::unique_ptr<VarDefList>> VarDefList
%nterm <std::unique_ptr<VarDef>> VarDef
%nterm <std::unique_ptr<FuncDef>> FuncDef
%nterm <std::unique_ptr<FuncParamList>> FuncParamList
%nterm <std::unique_ptr<FuncParam>> FuncParam
%nterm <std::unique_ptr<InitValList>> ConstInitValList InitValList
%nterm <std::unique_ptr<InitVal>> ConstInitVal InitVal
%nterm <std::unique_ptr<Block>> Block
%nterm <std::unique_ptr<BlockItemList>> BlockItemList
%nterm <std::unique_ptr<LVal>> LVal
%nterm <std::unique_ptr<ArrayList>> ExpList ConstExpList
%nterm <std::unique_ptr<FuncRParamList>> FuncRParamList
%nterm <std::unique_ptr<BaseAST>> PrimaryExp
%nterm <std::unique_ptr<UnaryExp>> UnaryExp
%nterm <std::unique_ptr<MulExp>> MulExp
%nterm <std::unique_ptr<AddExp>> AddExp Exp ConstExp
%nterm <std::unique_ptr<RelExp>> RelExp
%nterm <std::unique_ptr<EqExp>> EqExp
%nterm <std::unique_ptr<LAndExp>> LAndExp
%nterm <std::unique_ptr<LOrExp>> LOrExp

%start Start

%%

/* 解析成功后把根节点交给 main.cpp 里的全局变量 */
Start
    : CompUnit                  { ASTRoot = std::move($1); }
    ;

/* 左递归 + pushBack：保持源码顺序，且不会让分析栈无限增长 */
CompUnit
    : FuncDef                   { $$ = std::unique_ptr<CompUnit>(new CompUnit($1.release())); }
    | Decl                      { $$ = std::unique_ptr<CompUnit>(new CompUnit($1.release())); }
    | CompUnit FuncDef          { $$ = std::move($1); $$->pushBack($2.release()); }
    | CompUnit Decl             { $$ = std::move($1); $$->pushBack($2.release()); }
    ;

Type
    : INT                       { $$ = ASTType::makeScalar(ASTScalarKind::Int32); }
    | FLOAT                     { $$ = ASTType::makeScalar(ASTScalarKind::Float32); }
    | VOID                      { $$ = ASTType::makeScalar(ASTScalarKind::Void); }
    ;

/* ---------------- 声明 ---------------- */
Decl
    : ConstDecl                 { $$ = std::move($1); }
    | VarDecl                   { $$ = std::move($1); }
    ;

ConstDecl
    : CONST Type ConstDefList SEMICOLON
        { $$ = std::unique_ptr<BaseAST>(new ConstDecl($2, $3.release())); }
    ;

ConstDefList
    : ConstDef                          { $$ = std::unique_ptr<ConstDefList>(new ConstDefList($1.release())); }
    | ConstDefList COMMA ConstDef       { $$ = std::move($1); $$->pushBack($3.release()); }
    ;

/* 常量定义必须有初值；IDENT 是值类型，自动析构，不用手动 delete */
ConstDef
    : IDENT ASSIGN ConstInitVal
        { $$ = std::unique_ptr<ConstDef>(new ConstDef($1.text, nullptr, $3.release())); }
    | IDENT ConstExpList ASSIGN ConstInitVal
        { $$ = std::unique_ptr<ConstDef>(new ConstDef($1.text, $2.release(), $4.release())); }
    ;

VarDecl
    : Type VarDefList SEMICOLON
        { $$ = std::unique_ptr<BaseAST>(new VarDecl($1, $2.release())); }
    ;

VarDefList
    : VarDef                            { $$ = std::unique_ptr<VarDefList>(new VarDefList($1.release())); }
    | VarDefList COMMA VarDef           { $$ = std::move($1); $$->pushBack($3.release()); }
    ;

VarDef
    : IDENT                             { $$ = std::unique_ptr<VarDef>(new VarDef($1.text)); }
    | IDENT ASSIGN InitVal              { $$ = std::unique_ptr<VarDef>(new VarDef($1.text, nullptr, $3.release())); }
    | IDENT ConstExpList                { $$ = std::unique_ptr<VarDef>(new VarDef($1.text, $2.release(), nullptr)); }
    | IDENT ConstExpList ASSIGN InitVal { $$ = std::unique_ptr<VarDef>(new VarDef($1.text, $2.release(), $4.release())); }
    ;

/* 初值：表达式 / 空 {}（→ (InitVal)）/ 嵌套列表（→ (InitValList ...)），不展开不补零 */
ConstInitVal
    : ConstExp                          { $$ = std::unique_ptr<InitVal>(new InitVal($1.release())); }
    | LBRACE RBRACE                     { $$ = std::unique_ptr<InitVal>(new InitVal()); }
    | LBRACE ConstInitValList RBRACE    { $$ = std::unique_ptr<InitVal>(new InitVal($2.release())); }
    ;

ConstInitValList
    : ConstInitVal                              { $$ = std::unique_ptr<InitValList>(new InitValList($1.release())); }
    | ConstInitValList COMMA ConstInitVal       { $$ = std::move($1); $$->pushBack($3.release()); }
    ;

InitVal
    : Exp                               { $$ = std::unique_ptr<InitVal>(new InitVal($1.release())); }
    | LBRACE RBRACE                     { $$ = std::unique_ptr<InitVal>(new InitVal()); }
    | LBRACE InitValList RBRACE         { $$ = std::unique_ptr<InitVal>(new InitVal($2.release())); }
    ;

InitValList
    : InitVal                           { $$ = std::unique_ptr<InitValList>(new InitValList($1.release())); }
    | InitValList COMMA InitVal         { $$ = std::move($1); $$->pushBack($3.release()); }
    ;

/* ---------------- 函数 ---------------- */
FuncDef
    : Type IDENT LPAREN RPAREN Block
        { $$ = std::unique_ptr<FuncDef>(new FuncDef($1, $2.text, nullptr, $5.release())); }
    | Type IDENT LPAREN FuncParamList RPAREN Block
        { $$ = std::unique_ptr<FuncDef>(new FuncDef($1, $2.text, $4.release(), $6.release())); }
    ;

FuncParamList
    : FuncParam                         { $$ = std::unique_ptr<FuncParamList>(new FuncParamList($1.release())); }
    | FuncParamList COMMA FuncParam     { $$ = std::move($1); $$->pushBack($3.release()); }
    ;

/* 形参四种形态对应 AST 的四个构造函数（维度标记见 docs/AST_FORMAT.md） */
FuncParam
    : Type IDENT                            { $$ = std::unique_ptr<FuncParam>(new FuncParam($1, $2.text)); }
    | Type IDENT LBRACKET RBRACKET          { $$ = std::unique_ptr<FuncParam>(new FuncParam($1, $2.text, true)); }
    | Type IDENT ExpList                    { $$ = std::unique_ptr<FuncParam>(new FuncParam($1, $2.text, false, $3.release())); }
    | Type IDENT LBRACKET RBRACKET ExpList  { $$ = std::unique_ptr<FuncParam>(new FuncParam($1, $2.text, true, $5.release())); }
    ;

/* ---------------- 语句 ---------------- */
Block
    : LBRACE RBRACE                     { $$ = std::unique_ptr<Block>(new Block(nullptr)); }   /* 空块 → (Block) */
    | LBRACE BlockItemList RBRACE       { $$ = std::unique_ptr<Block>(new Block($2.release())); }
    ;

BlockItemList
    : BlockItem                         { $$ = std::unique_ptr<BlockItemList>(new BlockItemList($1.release())); }
    | BlockItemList BlockItem           { $$ = std::move($1); $$->pushBack($2.release()); }
    ;

BlockItem
    : Decl                              { $$ = std::move($1); }
    | Stmt                              { $$ = std::move($1); }
    ;

Stmt
    : LVal ASSIGN Exp SEMICOLON         { $$ = std::unique_ptr<BaseAST>(new AssignStmt($1.release(), $3.release())); }
    | SEMICOLON                         { $$ = std::unique_ptr<BaseAST>(new ExpStmt(nullptr)); }   /* 空语句 → (ExpStmt) */
    | Exp SEMICOLON                     { $$ = std::unique_ptr<BaseAST>(new ExpStmt($1.release())); }
    | Block                             { $$ = std::move($1); }
    | IF LPAREN LOrExp RPAREN Stmt      { $$ = std::unique_ptr<BaseAST>(new IfStmt($3.release(), $5.release())); }
    | IF LPAREN LOrExp RPAREN Stmt ELSE Stmt
                                        { $$ = std::unique_ptr<BaseAST>(new IfStmt($3.release(), $5.release(), $7.release())); }
    | WHILE LPAREN LOrExp RPAREN Stmt   { $$ = std::unique_ptr<BaseAST>(new WhileStmt($3.release(), $5.release())); }
    | BREAK SEMICOLON                   { $$ = std::unique_ptr<BaseAST>(new BreakStmt()); }
    | CONTINUE SEMICOLON                { $$ = std::unique_ptr<BaseAST>(new ContinueStmt()); }
    | RETURN SEMICOLON                  { $$ = std::unique_ptr<BaseAST>(new ReturnStmt()); }   /* 无返回值 → (ReturnStmt) */
    | RETURN Exp SEMICOLON              { $$ = std::unique_ptr<BaseAST>(new ReturnStmt($2.release())); }
    ;

/* ---------------- 表达式 ---------------- */
/* 括号不建节点：内层 AddExp 会成为外层 UnaryExp 的操作数，分组信息因此保留 */
PrimaryExp
    : LPAREN Exp RPAREN                 { $$ = std::move($2); }
    | LVal                              { $$ = std::move($1); }
    | INT_CONST                         { $$ = std::unique_ptr<BaseAST>(new ConValue<int>($1)); }
    | FLOAT_CONST                       { $$ = std::unique_ptr<BaseAST>(new ConValue<float>($1)); }
    | IDENT LPAREN RPAREN               { $$ = std::unique_ptr<BaseAST>(new FuncCall($1.text, $1.line)); }
    | IDENT LPAREN FuncRParamList RPAREN{ $$ = std::unique_ptr<BaseAST>(new FuncCall($1.text, $3.release(), $1.line)); }
    ;

LVal
    : IDENT                             { $$ = std::unique_ptr<LVal>(new LVal($1.text)); }
    | IDENT ExpList                     { $$ = std::unique_ptr<LVal>(new LVal($1.text, $2.release())); }
    ;

ExpList
    : LBRACKET Exp RBRACKET             { $$ = std::unique_ptr<ArrayList>(new ArrayList($2.release())); }
    | ExpList LBRACKET Exp RBRACKET     { $$ = std::move($1); $$->pushBack($3.release()); }
    ;

ConstExpList
    : LBRACKET ConstExp RBRACKET                { $$ = std::unique_ptr<ArrayList>(new ArrayList($2.release())); }
    | ConstExpList LBRACKET ConstExp RBRACKET   { $$ = std::move($1); $$->pushBack($3.release()); }
    ;

FuncRParamList
    : Exp                               { $$ = std::unique_ptr<FuncRParamList>(new FuncRParamList($1.release())); }
    | FuncRParamList COMMA Exp          { $$ = std::move($1); $$->pushBack($3.release()); }
    ;

/* 一元：右递归 + pushFront，操作符按“从外到内”排列，如 -+a → (Ops - +) */
UnaryExp
    : PrimaryExp                        { $$ = std::unique_ptr<UnaryExp>(new UnaryExp($1.release())); }
    | ADD UnaryExp                      { $$ = std::move($2); $$->pushFront(SY_ADD); }
    | SUB UnaryExp                      { $$ = std::move($2); $$->pushFront(SY_SUB); }
    | NOT UnaryExp                      { $$ = std::move($2); $$->pushFront(SY_NOT); }
    ;

/* 二元：左递归 + pushBack，同层链条平铺成 (Ops ...) 配 N 个操作数，如 a-b-c → (Ops - -) */
MulExp
    : UnaryExp                          { $$ = std::unique_ptr<MulExp>(new MulExp($1.release())); }
    | MulExp MUL UnaryExp               { $$ = std::move($1); $$->pushBack(SY_MUL); $$->pushBack($3.release()); }
    | MulExp DIV UnaryExp               { $$ = std::move($1); $$->pushBack(SY_DIV); $$->pushBack($3.release()); }
    | MulExp MOD UnaryExp               { $$ = std::move($1); $$->pushBack(SY_MOD); $$->pushBack($3.release()); }
    ;

AddExp
    : MulExp                            { $$ = std::unique_ptr<AddExp>(new AddExp($1.release())); }
    | AddExp ADD MulExp                 { $$ = std::move($1); $$->pushBack(SY_ADD); $$->pushBack($3.release()); }
    | AddExp SUB MulExp                 { $$ = std::move($1); $$->pushBack(SY_SUB); $$->pushBack($3.release()); }
    ;

RelExp
    : AddExp                            { $$ = std::unique_ptr<RelExp>(new RelExp($1.release())); }
    | RelExp LESS AddExp                { $$ = std::move($1); $$->pushBack(SY_LESS); $$->pushBack($3.release()); }
    | RelExp GREAT AddExp               { $$ = std::move($1); $$->pushBack(SY_GREAT); $$->pushBack($3.release()); }
    | RelExp LESSEQ AddExp              { $$ = std::move($1); $$->pushBack(SY_LESSEQ); $$->pushBack($3.release()); }
    | RelExp GREATEQ AddExp             { $$ = std::move($1); $$->pushBack(SY_GREATEQ); $$->pushBack($3.release()); }
    ;

EqExp
    : RelExp                            { $$ = std::unique_ptr<EqExp>(new EqExp($1.release())); }
    | EqExp EQ RelExp                   { $$ = std::move($1); $$->pushBack(SY_EQ); $$->pushBack($3.release()); }
    | EqExp NOTEQ RelExp                { $$ = std::move($1); $$->pushBack(SY_NOTEQ); $$->pushBack($3.release()); }
    ;

LAndExp
    : EqExp                             { $$ = std::unique_ptr<LAndExp>(new LAndExp($1.release())); }
    | LAndExp AND EqExp                 { $$ = std::move($1); $$->pushBack(SY_AND); $$->pushBack($3.release()); }
    ;

LOrExp
    : LAndExp                           { $$ = std::unique_ptr<LOrExp>(new LOrExp($1.release())); }
    | LOrExp OR LAndExp                 { $$ = std::move($1); $$->pushBack(SY_OR); $$->pushBack($3.release()); }
    ;

Exp
    : AddExp                            { $$ = std::move($1); }
    ;

ConstExp
    : AddExp                            { $$ = std::move($1); }
    ;

%%
