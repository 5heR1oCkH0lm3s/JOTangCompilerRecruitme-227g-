/*
 * Task1 词法分析器（Flex）—— SysY2022
 * 生成：flex --header-file=include/yacc/Flex.hpp --outfile=src/yacc/Flex.cpp src/yacc/sysy.l
 */
%option noyywrap
%option yylineno

%{
#include "yacc/Bison.hpp"   /* token 枚举与 make_XXX 工厂由 Bison 生成 */

#include <cerrno>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <string>

/* 扫描器的返回值就是一个“带语义值的 token 对象”。 */
#define YY_DECL yy::parser::symbol_type yylex()

/* 整数字面量：按进制解析；越界报错（不用 std::stoi：它不认进制且抛异常难控制）。 */
static int toInt(const char* text, int base) {
    errno = 0;
    unsigned long long value = std::strtoull(text, nullptr, base);
    if (errno == ERANGE || value > 0xFFFFFFFFull) {
        throw std::runtime_error(std::string("整数常量超出 32 位范围：") + text);
    }
    return static_cast<int>(static_cast<int32_t>(value));
}

/* 浮点字面量：strtof 同时支持十进制与十六进制（0x1.8p+1）。 */
static float toFloat(const char* text) {
    errno = 0;
    float value = std::strtof(text, nullptr);
    if (errno == ERANGE && (value == HUGE_VALF || value == -HUGE_VALF)) {
        throw std::runtime_error(std::string("浮点常量超出范围：") + text);
    }
    return value;
}
%}

WS        [ \t\r\n]+
DIGIT     [0-9]
HEXDIG    [0-9a-fA-F]
EXP       [eE][+-]?{DIGIT}+
HEXEXP    [pP][+-]?{DIGIT}+
IDENT     [a-zA-Z_][a-zA-Z0-9_]*

DEC_FLOAT (({DIGIT}+"."{DIGIT}*|"."{DIGIT}+)({EXP})?|{DIGIT}+{EXP})
HEX_FLOAT 0[xX]({HEXDIG}+"."{HEXDIG}*|"."{HEXDIG}+|{HEXDIG}+){HEXEXP}
HEX_INT   0[xX]{HEXDIG}+
OCT_INT   0[0-7]*
DEC_INT   [1-9][0-9]*

/* 提示：规则段里不能写独立的多行注释，注释只能放定义段或动作内部。 */
%x COMMENT

%%

{WS}              { }
"//"[^\n]*        { }

"/*"              { BEGIN(COMMENT); }        /* 多行注释用状态机（下面四条） */
<COMMENT>"*/"     { BEGIN(INITIAL); }
<COMMENT>\n       { }                        /* 换行时 yylineno 由 Flex 自动累加 */
<COMMENT>.        { }                        /* 注释不嵌套，只找第一个 */ 
<COMMENT><<EOF>>  { throw std::runtime_error("词法错误：多行注释没有闭合"); }

"int"       { return yy::parser::make_INT(); }
"float"     { return yy::parser::make_FLOAT(); }
"void"      { return yy::parser::make_VOID(); }
"const"     { return yy::parser::make_CONST(); }
"return"    { return yy::parser::make_RETURN(); }
"if"        { return yy::parser::make_IF(); }
"else"      { return yy::parser::make_ELSE(); }
"while"     { return yy::parser::make_WHILE(); }
"break"     { return yy::parser::make_BREAK(); }
"continue"  { return yy::parser::make_CONTINUE(); }

"("   { return yy::parser::make_LPAREN(); }
")"   { return yy::parser::make_RPAREN(); }
"{"   { return yy::parser::make_LBRACE(); }
"}"   { return yy::parser::make_RBRACE(); }
"["   { return yy::parser::make_LBRACKET(); }
"]"   { return yy::parser::make_RBRACKET(); }
";"   { return yy::parser::make_SEMICOLON(); }
","   { return yy::parser::make_COMMA(); }

"||"  { return yy::parser::make_OR(); }
"&&"  { return yy::parser::make_AND(); }
"=="  { return yy::parser::make_EQ(); }
"!="  { return yy::parser::make_NOTEQ(); }
"<="  { return yy::parser::make_LESSEQ(); }
">="  { return yy::parser::make_GREATEQ(); }
"<"   { return yy::parser::make_LESS(); }
">"   { return yy::parser::make_GREAT(); }
"!"   { return yy::parser::make_NOT(); }
"+"   { return yy::parser::make_ADD(); }
"-"   { return yy::parser::make_SUB(); }
"*"   { return yy::parser::make_MUL(); }
"/"   { return yy::parser::make_DIV(); }
"%"   { return yy::parser::make_MOD(); }
"="   { return yy::parser::make_ASSIGN(); }

{HEX_FLOAT}  { return yy::parser::make_FLOAT_CONST(toFloat(yytext)); }
{DEC_FLOAT}  { return yy::parser::make_FLOAT_CONST(toFloat(yytext)); }
{HEX_INT}    { return yy::parser::make_INT_CONST(toInt(yytext, 16)); }
{OCT_INT}    { return yy::parser::make_INT_CONST(toInt(yytext, 8)); }
{DEC_INT}    { return yy::parser::make_INT_CONST(toInt(yytext, 10)); }

{IDENT}  {
             /* 名字要复制走（yytext 缓冲区会被复用），行号在识别到名字的这一刻锁定 */
             return yy::parser::make_IDENT(LocatedIdentifier{std::string(yytext), yylineno});
         }

<<EOF>>  { return yy::parser::symbol_type(0); }   /* token 0 就是“输入结束” */

.        {
             throw std::runtime_error("词法错误：第 " + std::to_string(yylineno) +
                                      " 行出现无法识别的字符 '" + std::string(yytext) + "'");
         }

%%
