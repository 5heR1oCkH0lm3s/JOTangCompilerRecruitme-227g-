// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "Bison.hpp"


// Unqualified %code blocks.
#line 32 "src/yacc/sysy.y"

    #include "Frontend.hpp"   /* extern std::unique_ptr<CompUnit> ASTRoot; */

    #include <iostream>

    extern yy::parser::symbol_type yylex();   /* token 由 Flex 提供 */
    extern int yylineno;                      /* 行号由 Flex 维护 */

    void yy::parser::error(const std::string& msg) {
        std::cerr << "语法错误（第 " << yylineno << " 行）：" << msg << '\n';
    }

#line 59 "src/yacc/Bison.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 132 "src/yacc/Bison.cpp"

  /// Build a parser object.
  parser::parser ()
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr)
#else

#endif
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_Type: // Type
        value.YY_MOVE_OR_COPY< ASTType > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDENT: // IDENT
        value.YY_MOVE_OR_COPY< LocatedIdentifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT_CONST: // FLOAT_CONST
        value.YY_MOVE_OR_COPY< float > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INT_CONST: // INT_CONST
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_AddExp: // AddExp
      case symbol_kind::S_Exp: // Exp
      case symbol_kind::S_ConstExp: // ConstExp
        value.YY_MOVE_OR_COPY< std::unique_ptr<AddExp> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ExpList: // ExpList
      case symbol_kind::S_ConstExpList: // ConstExpList
        value.YY_MOVE_OR_COPY< std::unique_ptr<ArrayList> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Decl: // Decl
      case symbol_kind::S_ConstDecl: // ConstDecl
      case symbol_kind::S_VarDecl: // VarDecl
      case symbol_kind::S_BlockItem: // BlockItem
      case symbol_kind::S_Stmt: // Stmt
      case symbol_kind::S_PrimaryExp: // PrimaryExp
        value.YY_MOVE_OR_COPY< std::unique_ptr<BaseAST> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Block: // Block
        value.YY_MOVE_OR_COPY< std::unique_ptr<Block> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_BlockItemList: // BlockItemList
        value.YY_MOVE_OR_COPY< std::unique_ptr<BlockItemList> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_CompUnit: // CompUnit
        value.YY_MOVE_OR_COPY< std::unique_ptr<CompUnit> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ConstDef: // ConstDef
        value.YY_MOVE_OR_COPY< std::unique_ptr<ConstDef> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ConstDefList: // ConstDefList
        value.YY_MOVE_OR_COPY< std::unique_ptr<ConstDefList> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_EqExp: // EqExp
        value.YY_MOVE_OR_COPY< std::unique_ptr<EqExp> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FuncDef: // FuncDef
        value.YY_MOVE_OR_COPY< std::unique_ptr<FuncDef> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FuncParam: // FuncParam
        value.YY_MOVE_OR_COPY< std::unique_ptr<FuncParam> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FuncParamList: // FuncParamList
        value.YY_MOVE_OR_COPY< std::unique_ptr<FuncParamList> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FuncRParamList: // FuncRParamList
        value.YY_MOVE_OR_COPY< std::unique_ptr<FuncRParamList> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ConstInitVal: // ConstInitVal
      case symbol_kind::S_InitVal: // InitVal
        value.YY_MOVE_OR_COPY< std::unique_ptr<InitVal> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ConstInitValList: // ConstInitValList
      case symbol_kind::S_InitValList: // InitValList
        value.YY_MOVE_OR_COPY< std::unique_ptr<InitValList> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_LAndExp: // LAndExp
        value.YY_MOVE_OR_COPY< std::unique_ptr<LAndExp> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_LOrExp: // LOrExp
        value.YY_MOVE_OR_COPY< std::unique_ptr<LOrExp> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_LVal: // LVal
        value.YY_MOVE_OR_COPY< std::unique_ptr<LVal> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_MulExp: // MulExp
        value.YY_MOVE_OR_COPY< std::unique_ptr<MulExp> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_RelExp: // RelExp
        value.YY_MOVE_OR_COPY< std::unique_ptr<RelExp> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_UnaryExp: // UnaryExp
        value.YY_MOVE_OR_COPY< std::unique_ptr<UnaryExp> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_VarDef: // VarDef
        value.YY_MOVE_OR_COPY< std::unique_ptr<VarDef> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_VarDefList: // VarDefList
        value.YY_MOVE_OR_COPY< std::unique_ptr<VarDefList> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.kind ())
    {
      case symbol_kind::S_Type: // Type
        value.move< ASTType > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDENT: // IDENT
        value.move< LocatedIdentifier > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT_CONST: // FLOAT_CONST
        value.move< float > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INT_CONST: // INT_CONST
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_AddExp: // AddExp
      case symbol_kind::S_Exp: // Exp
      case symbol_kind::S_ConstExp: // ConstExp
        value.move< std::unique_ptr<AddExp> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ExpList: // ExpList
      case symbol_kind::S_ConstExpList: // ConstExpList
        value.move< std::unique_ptr<ArrayList> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Decl: // Decl
      case symbol_kind::S_ConstDecl: // ConstDecl
      case symbol_kind::S_VarDecl: // VarDecl
      case symbol_kind::S_BlockItem: // BlockItem
      case symbol_kind::S_Stmt: // Stmt
      case symbol_kind::S_PrimaryExp: // PrimaryExp
        value.move< std::unique_ptr<BaseAST> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Block: // Block
        value.move< std::unique_ptr<Block> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_BlockItemList: // BlockItemList
        value.move< std::unique_ptr<BlockItemList> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_CompUnit: // CompUnit
        value.move< std::unique_ptr<CompUnit> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ConstDef: // ConstDef
        value.move< std::unique_ptr<ConstDef> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ConstDefList: // ConstDefList
        value.move< std::unique_ptr<ConstDefList> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_EqExp: // EqExp
        value.move< std::unique_ptr<EqExp> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FuncDef: // FuncDef
        value.move< std::unique_ptr<FuncDef> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FuncParam: // FuncParam
        value.move< std::unique_ptr<FuncParam> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FuncParamList: // FuncParamList
        value.move< std::unique_ptr<FuncParamList> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FuncRParamList: // FuncRParamList
        value.move< std::unique_ptr<FuncRParamList> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ConstInitVal: // ConstInitVal
      case symbol_kind::S_InitVal: // InitVal
        value.move< std::unique_ptr<InitVal> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ConstInitValList: // ConstInitValList
      case symbol_kind::S_InitValList: // InitValList
        value.move< std::unique_ptr<InitValList> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_LAndExp: // LAndExp
        value.move< std::unique_ptr<LAndExp> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_LOrExp: // LOrExp
        value.move< std::unique_ptr<LOrExp> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_LVal: // LVal
        value.move< std::unique_ptr<LVal> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_MulExp: // MulExp
        value.move< std::unique_ptr<MulExp> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_RelExp: // RelExp
        value.move< std::unique_ptr<RelExp> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_UnaryExp: // UnaryExp
        value.move< std::unique_ptr<UnaryExp> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_VarDef: // VarDef
        value.move< std::unique_ptr<VarDef> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_VarDefList: // VarDefList
        value.move< std::unique_ptr<VarDefList> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_Type: // Type
        value.copy< ASTType > (that.value);
        break;

      case symbol_kind::S_IDENT: // IDENT
        value.copy< LocatedIdentifier > (that.value);
        break;

      case symbol_kind::S_FLOAT_CONST: // FLOAT_CONST
        value.copy< float > (that.value);
        break;

      case symbol_kind::S_INT_CONST: // INT_CONST
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_AddExp: // AddExp
      case symbol_kind::S_Exp: // Exp
      case symbol_kind::S_ConstExp: // ConstExp
        value.copy< std::unique_ptr<AddExp> > (that.value);
        break;

      case symbol_kind::S_ExpList: // ExpList
      case symbol_kind::S_ConstExpList: // ConstExpList
        value.copy< std::unique_ptr<ArrayList> > (that.value);
        break;

      case symbol_kind::S_Decl: // Decl
      case symbol_kind::S_ConstDecl: // ConstDecl
      case symbol_kind::S_VarDecl: // VarDecl
      case symbol_kind::S_BlockItem: // BlockItem
      case symbol_kind::S_Stmt: // Stmt
      case symbol_kind::S_PrimaryExp: // PrimaryExp
        value.copy< std::unique_ptr<BaseAST> > (that.value);
        break;

      case symbol_kind::S_Block: // Block
        value.copy< std::unique_ptr<Block> > (that.value);
        break;

      case symbol_kind::S_BlockItemList: // BlockItemList
        value.copy< std::unique_ptr<BlockItemList> > (that.value);
        break;

      case symbol_kind::S_CompUnit: // CompUnit
        value.copy< std::unique_ptr<CompUnit> > (that.value);
        break;

      case symbol_kind::S_ConstDef: // ConstDef
        value.copy< std::unique_ptr<ConstDef> > (that.value);
        break;

      case symbol_kind::S_ConstDefList: // ConstDefList
        value.copy< std::unique_ptr<ConstDefList> > (that.value);
        break;

      case symbol_kind::S_EqExp: // EqExp
        value.copy< std::unique_ptr<EqExp> > (that.value);
        break;

      case symbol_kind::S_FuncDef: // FuncDef
        value.copy< std::unique_ptr<FuncDef> > (that.value);
        break;

      case symbol_kind::S_FuncParam: // FuncParam
        value.copy< std::unique_ptr<FuncParam> > (that.value);
        break;

      case symbol_kind::S_FuncParamList: // FuncParamList
        value.copy< std::unique_ptr<FuncParamList> > (that.value);
        break;

      case symbol_kind::S_FuncRParamList: // FuncRParamList
        value.copy< std::unique_ptr<FuncRParamList> > (that.value);
        break;

      case symbol_kind::S_ConstInitVal: // ConstInitVal
      case symbol_kind::S_InitVal: // InitVal
        value.copy< std::unique_ptr<InitVal> > (that.value);
        break;

      case symbol_kind::S_ConstInitValList: // ConstInitValList
      case symbol_kind::S_InitValList: // InitValList
        value.copy< std::unique_ptr<InitValList> > (that.value);
        break;

      case symbol_kind::S_LAndExp: // LAndExp
        value.copy< std::unique_ptr<LAndExp> > (that.value);
        break;

      case symbol_kind::S_LOrExp: // LOrExp
        value.copy< std::unique_ptr<LOrExp> > (that.value);
        break;

      case symbol_kind::S_LVal: // LVal
        value.copy< std::unique_ptr<LVal> > (that.value);
        break;

      case symbol_kind::S_MulExp: // MulExp
        value.copy< std::unique_ptr<MulExp> > (that.value);
        break;

      case symbol_kind::S_RelExp: // RelExp
        value.copy< std::unique_ptr<RelExp> > (that.value);
        break;

      case symbol_kind::S_UnaryExp: // UnaryExp
        value.copy< std::unique_ptr<UnaryExp> > (that.value);
        break;

      case symbol_kind::S_VarDef: // VarDef
        value.copy< std::unique_ptr<VarDef> > (that.value);
        break;

      case symbol_kind::S_VarDefList: // VarDefList
        value.copy< std::unique_ptr<VarDefList> > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_Type: // Type
        value.move< ASTType > (that.value);
        break;

      case symbol_kind::S_IDENT: // IDENT
        value.move< LocatedIdentifier > (that.value);
        break;

      case symbol_kind::S_FLOAT_CONST: // FLOAT_CONST
        value.move< float > (that.value);
        break;

      case symbol_kind::S_INT_CONST: // INT_CONST
        value.move< int > (that.value);
        break;

      case symbol_kind::S_AddExp: // AddExp
      case symbol_kind::S_Exp: // Exp
      case symbol_kind::S_ConstExp: // ConstExp
        value.move< std::unique_ptr<AddExp> > (that.value);
        break;

      case symbol_kind::S_ExpList: // ExpList
      case symbol_kind::S_ConstExpList: // ConstExpList
        value.move< std::unique_ptr<ArrayList> > (that.value);
        break;

      case symbol_kind::S_Decl: // Decl
      case symbol_kind::S_ConstDecl: // ConstDecl
      case symbol_kind::S_VarDecl: // VarDecl
      case symbol_kind::S_BlockItem: // BlockItem
      case symbol_kind::S_Stmt: // Stmt
      case symbol_kind::S_PrimaryExp: // PrimaryExp
        value.move< std::unique_ptr<BaseAST> > (that.value);
        break;

      case symbol_kind::S_Block: // Block
        value.move< std::unique_ptr<Block> > (that.value);
        break;

      case symbol_kind::S_BlockItemList: // BlockItemList
        value.move< std::unique_ptr<BlockItemList> > (that.value);
        break;

      case symbol_kind::S_CompUnit: // CompUnit
        value.move< std::unique_ptr<CompUnit> > (that.value);
        break;

      case symbol_kind::S_ConstDef: // ConstDef
        value.move< std::unique_ptr<ConstDef> > (that.value);
        break;

      case symbol_kind::S_ConstDefList: // ConstDefList
        value.move< std::unique_ptr<ConstDefList> > (that.value);
        break;

      case symbol_kind::S_EqExp: // EqExp
        value.move< std::unique_ptr<EqExp> > (that.value);
        break;

      case symbol_kind::S_FuncDef: // FuncDef
        value.move< std::unique_ptr<FuncDef> > (that.value);
        break;

      case symbol_kind::S_FuncParam: // FuncParam
        value.move< std::unique_ptr<FuncParam> > (that.value);
        break;

      case symbol_kind::S_FuncParamList: // FuncParamList
        value.move< std::unique_ptr<FuncParamList> > (that.value);
        break;

      case symbol_kind::S_FuncRParamList: // FuncRParamList
        value.move< std::unique_ptr<FuncRParamList> > (that.value);
        break;

      case symbol_kind::S_ConstInitVal: // ConstInitVal
      case symbol_kind::S_InitVal: // InitVal
        value.move< std::unique_ptr<InitVal> > (that.value);
        break;

      case symbol_kind::S_ConstInitValList: // ConstInitValList
      case symbol_kind::S_InitValList: // InitValList
        value.move< std::unique_ptr<InitValList> > (that.value);
        break;

      case symbol_kind::S_LAndExp: // LAndExp
        value.move< std::unique_ptr<LAndExp> > (that.value);
        break;

      case symbol_kind::S_LOrExp: // LOrExp
        value.move< std::unique_ptr<LOrExp> > (that.value);
        break;

      case symbol_kind::S_LVal: // LVal
        value.move< std::unique_ptr<LVal> > (that.value);
        break;

      case symbol_kind::S_MulExp: // MulExp
        value.move< std::unique_ptr<MulExp> > (that.value);
        break;

      case symbol_kind::S_RelExp: // RelExp
        value.move< std::unique_ptr<RelExp> > (that.value);
        break;

      case symbol_kind::S_UnaryExp: // UnaryExp
        value.move< std::unique_ptr<UnaryExp> > (that.value);
        break;

      case symbol_kind::S_VarDef: // VarDef
        value.move< std::unique_ptr<VarDef> > (that.value);
        break;

      case symbol_kind::S_VarDefList: // VarDefList
        value.move< std::unique_ptr<VarDefList> > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " (";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex ());
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_Type: // Type
        yylhs.value.emplace< ASTType > ();
        break;

      case symbol_kind::S_IDENT: // IDENT
        yylhs.value.emplace< LocatedIdentifier > ();
        break;

      case symbol_kind::S_FLOAT_CONST: // FLOAT_CONST
        yylhs.value.emplace< float > ();
        break;

      case symbol_kind::S_INT_CONST: // INT_CONST
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_AddExp: // AddExp
      case symbol_kind::S_Exp: // Exp
      case symbol_kind::S_ConstExp: // ConstExp
        yylhs.value.emplace< std::unique_ptr<AddExp> > ();
        break;

      case symbol_kind::S_ExpList: // ExpList
      case symbol_kind::S_ConstExpList: // ConstExpList
        yylhs.value.emplace< std::unique_ptr<ArrayList> > ();
        break;

      case symbol_kind::S_Decl: // Decl
      case symbol_kind::S_ConstDecl: // ConstDecl
      case symbol_kind::S_VarDecl: // VarDecl
      case symbol_kind::S_BlockItem: // BlockItem
      case symbol_kind::S_Stmt: // Stmt
      case symbol_kind::S_PrimaryExp: // PrimaryExp
        yylhs.value.emplace< std::unique_ptr<BaseAST> > ();
        break;

      case symbol_kind::S_Block: // Block
        yylhs.value.emplace< std::unique_ptr<Block> > ();
        break;

      case symbol_kind::S_BlockItemList: // BlockItemList
        yylhs.value.emplace< std::unique_ptr<BlockItemList> > ();
        break;

      case symbol_kind::S_CompUnit: // CompUnit
        yylhs.value.emplace< std::unique_ptr<CompUnit> > ();
        break;

      case symbol_kind::S_ConstDef: // ConstDef
        yylhs.value.emplace< std::unique_ptr<ConstDef> > ();
        break;

      case symbol_kind::S_ConstDefList: // ConstDefList
        yylhs.value.emplace< std::unique_ptr<ConstDefList> > ();
        break;

      case symbol_kind::S_EqExp: // EqExp
        yylhs.value.emplace< std::unique_ptr<EqExp> > ();
        break;

      case symbol_kind::S_FuncDef: // FuncDef
        yylhs.value.emplace< std::unique_ptr<FuncDef> > ();
        break;

      case symbol_kind::S_FuncParam: // FuncParam
        yylhs.value.emplace< std::unique_ptr<FuncParam> > ();
        break;

      case symbol_kind::S_FuncParamList: // FuncParamList
        yylhs.value.emplace< std::unique_ptr<FuncParamList> > ();
        break;

      case symbol_kind::S_FuncRParamList: // FuncRParamList
        yylhs.value.emplace< std::unique_ptr<FuncRParamList> > ();
        break;

      case symbol_kind::S_ConstInitVal: // ConstInitVal
      case symbol_kind::S_InitVal: // InitVal
        yylhs.value.emplace< std::unique_ptr<InitVal> > ();
        break;

      case symbol_kind::S_ConstInitValList: // ConstInitValList
      case symbol_kind::S_InitValList: // InitValList
        yylhs.value.emplace< std::unique_ptr<InitValList> > ();
        break;

      case symbol_kind::S_LAndExp: // LAndExp
        yylhs.value.emplace< std::unique_ptr<LAndExp> > ();
        break;

      case symbol_kind::S_LOrExp: // LOrExp
        yylhs.value.emplace< std::unique_ptr<LOrExp> > ();
        break;

      case symbol_kind::S_LVal: // LVal
        yylhs.value.emplace< std::unique_ptr<LVal> > ();
        break;

      case symbol_kind::S_MulExp: // MulExp
        yylhs.value.emplace< std::unique_ptr<MulExp> > ();
        break;

      case symbol_kind::S_RelExp: // RelExp
        yylhs.value.emplace< std::unique_ptr<RelExp> > ();
        break;

      case symbol_kind::S_UnaryExp: // UnaryExp
        yylhs.value.emplace< std::unique_ptr<UnaryExp> > ();
        break;

      case symbol_kind::S_VarDef: // VarDef
        yylhs.value.emplace< std::unique_ptr<VarDef> > ();
        break;

      case symbol_kind::S_VarDefList: // VarDefList
        yylhs.value.emplace< std::unique_ptr<VarDefList> > ();
        break;

      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // Start: CompUnit
#line 89 "src/yacc/sysy.y"
                                { ASTRoot = std::move(yystack_[0].value.as < std::unique_ptr<CompUnit> > ()); }
#line 1103 "src/yacc/Bison.cpp"
    break;

  case 3: // CompUnit: FuncDef
#line 94 "src/yacc/sysy.y"
                                { yylhs.value.as < std::unique_ptr<CompUnit> > () = std::unique_ptr<CompUnit>(new CompUnit(yystack_[0].value.as < std::unique_ptr<FuncDef> > ().release())); }
#line 1109 "src/yacc/Bison.cpp"
    break;

  case 4: // CompUnit: Decl
#line 95 "src/yacc/sysy.y"
                                { yylhs.value.as < std::unique_ptr<CompUnit> > () = std::unique_ptr<CompUnit>(new CompUnit(yystack_[0].value.as < std::unique_ptr<BaseAST> > ().release())); }
#line 1115 "src/yacc/Bison.cpp"
    break;

  case 5: // CompUnit: CompUnit FuncDef
#line 96 "src/yacc/sysy.y"
                                { yylhs.value.as < std::unique_ptr<CompUnit> > () = std::move(yystack_[1].value.as < std::unique_ptr<CompUnit> > ()); yylhs.value.as < std::unique_ptr<CompUnit> > ()->pushBack(yystack_[0].value.as < std::unique_ptr<FuncDef> > ().release()); }
#line 1121 "src/yacc/Bison.cpp"
    break;

  case 6: // CompUnit: CompUnit Decl
#line 97 "src/yacc/sysy.y"
                                { yylhs.value.as < std::unique_ptr<CompUnit> > () = std::move(yystack_[1].value.as < std::unique_ptr<CompUnit> > ()); yylhs.value.as < std::unique_ptr<CompUnit> > ()->pushBack(yystack_[0].value.as < std::unique_ptr<BaseAST> > ().release()); }
#line 1127 "src/yacc/Bison.cpp"
    break;

  case 7: // Type: INT
#line 101 "src/yacc/sysy.y"
                                { yylhs.value.as < ASTType > () = ASTType::makeScalar(ASTScalarKind::Int32); }
#line 1133 "src/yacc/Bison.cpp"
    break;

  case 8: // Type: FLOAT
#line 102 "src/yacc/sysy.y"
                                { yylhs.value.as < ASTType > () = ASTType::makeScalar(ASTScalarKind::Float32); }
#line 1139 "src/yacc/Bison.cpp"
    break;

  case 9: // Type: VOID
#line 103 "src/yacc/sysy.y"
                                { yylhs.value.as < ASTType > () = ASTType::makeScalar(ASTScalarKind::Void); }
#line 1145 "src/yacc/Bison.cpp"
    break;

  case 10: // Decl: ConstDecl
#line 108 "src/yacc/sysy.y"
                                { yylhs.value.as < std::unique_ptr<BaseAST> > () = std::move(yystack_[0].value.as < std::unique_ptr<BaseAST> > ()); }
#line 1151 "src/yacc/Bison.cpp"
    break;

  case 11: // Decl: VarDecl
#line 109 "src/yacc/sysy.y"
                                { yylhs.value.as < std::unique_ptr<BaseAST> > () = std::move(yystack_[0].value.as < std::unique_ptr<BaseAST> > ()); }
#line 1157 "src/yacc/Bison.cpp"
    break;

  case 12: // ConstDecl: CONST Type ConstDefList SEMICOLON
#line 114 "src/yacc/sysy.y"
        { yylhs.value.as < std::unique_ptr<BaseAST> > () = std::unique_ptr<BaseAST>(new ConstDecl(yystack_[2].value.as < ASTType > (), yystack_[1].value.as < std::unique_ptr<ConstDefList> > ().release())); }
#line 1163 "src/yacc/Bison.cpp"
    break;

  case 13: // ConstDefList: ConstDef
#line 118 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<ConstDefList> > () = std::unique_ptr<ConstDefList>(new ConstDefList(yystack_[0].value.as < std::unique_ptr<ConstDef> > ().release())); }
#line 1169 "src/yacc/Bison.cpp"
    break;

  case 14: // ConstDefList: ConstDefList COMMA ConstDef
#line 119 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<ConstDefList> > () = std::move(yystack_[2].value.as < std::unique_ptr<ConstDefList> > ()); yylhs.value.as < std::unique_ptr<ConstDefList> > ()->pushBack(yystack_[0].value.as < std::unique_ptr<ConstDef> > ().release()); }
#line 1175 "src/yacc/Bison.cpp"
    break;

  case 15: // ConstDef: IDENT ASSIGN ConstInitVal
#line 125 "src/yacc/sysy.y"
        { yylhs.value.as < std::unique_ptr<ConstDef> > () = std::unique_ptr<ConstDef>(new ConstDef(yystack_[2].value.as < LocatedIdentifier > ().text, nullptr, yystack_[0].value.as < std::unique_ptr<InitVal> > ().release())); }
#line 1181 "src/yacc/Bison.cpp"
    break;

  case 16: // ConstDef: IDENT ConstExpList ASSIGN ConstInitVal
#line 127 "src/yacc/sysy.y"
        { yylhs.value.as < std::unique_ptr<ConstDef> > () = std::unique_ptr<ConstDef>(new ConstDef(yystack_[3].value.as < LocatedIdentifier > ().text, yystack_[2].value.as < std::unique_ptr<ArrayList> > ().release(), yystack_[0].value.as < std::unique_ptr<InitVal> > ().release())); }
#line 1187 "src/yacc/Bison.cpp"
    break;

  case 17: // VarDecl: Type VarDefList SEMICOLON
#line 132 "src/yacc/sysy.y"
        { yylhs.value.as < std::unique_ptr<BaseAST> > () = std::unique_ptr<BaseAST>(new VarDecl(yystack_[2].value.as < ASTType > (), yystack_[1].value.as < std::unique_ptr<VarDefList> > ().release())); }
#line 1193 "src/yacc/Bison.cpp"
    break;

  case 18: // VarDefList: VarDef
#line 136 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<VarDefList> > () = std::unique_ptr<VarDefList>(new VarDefList(yystack_[0].value.as < std::unique_ptr<VarDef> > ().release())); }
#line 1199 "src/yacc/Bison.cpp"
    break;

  case 19: // VarDefList: VarDefList COMMA VarDef
#line 137 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<VarDefList> > () = std::move(yystack_[2].value.as < std::unique_ptr<VarDefList> > ()); yylhs.value.as < std::unique_ptr<VarDefList> > ()->pushBack(yystack_[0].value.as < std::unique_ptr<VarDef> > ().release()); }
#line 1205 "src/yacc/Bison.cpp"
    break;

  case 20: // VarDef: IDENT
#line 141 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<VarDef> > () = std::unique_ptr<VarDef>(new VarDef(yystack_[0].value.as < LocatedIdentifier > ().text)); }
#line 1211 "src/yacc/Bison.cpp"
    break;

  case 21: // VarDef: IDENT ASSIGN InitVal
#line 142 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<VarDef> > () = std::unique_ptr<VarDef>(new VarDef(yystack_[2].value.as < LocatedIdentifier > ().text, nullptr, yystack_[0].value.as < std::unique_ptr<InitVal> > ().release())); }
#line 1217 "src/yacc/Bison.cpp"
    break;

  case 22: // VarDef: IDENT ConstExpList
#line 143 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<VarDef> > () = std::unique_ptr<VarDef>(new VarDef(yystack_[1].value.as < LocatedIdentifier > ().text, yystack_[0].value.as < std::unique_ptr<ArrayList> > ().release(), nullptr)); }
#line 1223 "src/yacc/Bison.cpp"
    break;

  case 23: // VarDef: IDENT ConstExpList ASSIGN InitVal
#line 144 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<VarDef> > () = std::unique_ptr<VarDef>(new VarDef(yystack_[3].value.as < LocatedIdentifier > ().text, yystack_[2].value.as < std::unique_ptr<ArrayList> > ().release(), yystack_[0].value.as < std::unique_ptr<InitVal> > ().release())); }
#line 1229 "src/yacc/Bison.cpp"
    break;

  case 24: // ConstInitVal: ConstExp
#line 149 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<InitVal> > () = std::unique_ptr<InitVal>(new InitVal(yystack_[0].value.as < std::unique_ptr<AddExp> > ().release())); }
#line 1235 "src/yacc/Bison.cpp"
    break;

  case 25: // ConstInitVal: LBRACE RBRACE
#line 150 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<InitVal> > () = std::unique_ptr<InitVal>(new InitVal()); }
#line 1241 "src/yacc/Bison.cpp"
    break;

  case 26: // ConstInitVal: LBRACE ConstInitValList RBRACE
#line 151 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<InitVal> > () = std::unique_ptr<InitVal>(new InitVal(yystack_[1].value.as < std::unique_ptr<InitValList> > ().release())); }
#line 1247 "src/yacc/Bison.cpp"
    break;

  case 27: // ConstInitValList: ConstInitVal
#line 155 "src/yacc/sysy.y"
                                                { yylhs.value.as < std::unique_ptr<InitValList> > () = std::unique_ptr<InitValList>(new InitValList(yystack_[0].value.as < std::unique_ptr<InitVal> > ().release())); }
#line 1253 "src/yacc/Bison.cpp"
    break;

  case 28: // ConstInitValList: ConstInitValList COMMA ConstInitVal
#line 156 "src/yacc/sysy.y"
                                                { yylhs.value.as < std::unique_ptr<InitValList> > () = std::move(yystack_[2].value.as < std::unique_ptr<InitValList> > ()); yylhs.value.as < std::unique_ptr<InitValList> > ()->pushBack(yystack_[0].value.as < std::unique_ptr<InitVal> > ().release()); }
#line 1259 "src/yacc/Bison.cpp"
    break;

  case 29: // InitVal: Exp
#line 160 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<InitVal> > () = std::unique_ptr<InitVal>(new InitVal(yystack_[0].value.as < std::unique_ptr<AddExp> > ().release())); }
#line 1265 "src/yacc/Bison.cpp"
    break;

  case 30: // InitVal: LBRACE RBRACE
#line 161 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<InitVal> > () = std::unique_ptr<InitVal>(new InitVal()); }
#line 1271 "src/yacc/Bison.cpp"
    break;

  case 31: // InitVal: LBRACE InitValList RBRACE
#line 162 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<InitVal> > () = std::unique_ptr<InitVal>(new InitVal(yystack_[1].value.as < std::unique_ptr<InitValList> > ().release())); }
#line 1277 "src/yacc/Bison.cpp"
    break;

  case 32: // InitValList: InitVal
#line 166 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<InitValList> > () = std::unique_ptr<InitValList>(new InitValList(yystack_[0].value.as < std::unique_ptr<InitVal> > ().release())); }
#line 1283 "src/yacc/Bison.cpp"
    break;

  case 33: // InitValList: InitValList COMMA InitVal
#line 167 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<InitValList> > () = std::move(yystack_[2].value.as < std::unique_ptr<InitValList> > ()); yylhs.value.as < std::unique_ptr<InitValList> > ()->pushBack(yystack_[0].value.as < std::unique_ptr<InitVal> > ().release()); }
#line 1289 "src/yacc/Bison.cpp"
    break;

  case 34: // FuncDef: Type IDENT LPAREN RPAREN Block
#line 173 "src/yacc/sysy.y"
        { yylhs.value.as < std::unique_ptr<FuncDef> > () = std::unique_ptr<FuncDef>(new FuncDef(yystack_[4].value.as < ASTType > (), yystack_[3].value.as < LocatedIdentifier > ().text, nullptr, yystack_[0].value.as < std::unique_ptr<Block> > ().release())); }
#line 1295 "src/yacc/Bison.cpp"
    break;

  case 35: // FuncDef: Type IDENT LPAREN FuncParamList RPAREN Block
#line 175 "src/yacc/sysy.y"
        { yylhs.value.as < std::unique_ptr<FuncDef> > () = std::unique_ptr<FuncDef>(new FuncDef(yystack_[5].value.as < ASTType > (), yystack_[4].value.as < LocatedIdentifier > ().text, yystack_[2].value.as < std::unique_ptr<FuncParamList> > ().release(), yystack_[0].value.as < std::unique_ptr<Block> > ().release())); }
#line 1301 "src/yacc/Bison.cpp"
    break;

  case 36: // FuncParamList: FuncParam
#line 179 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<FuncParamList> > () = std::unique_ptr<FuncParamList>(new FuncParamList(yystack_[0].value.as < std::unique_ptr<FuncParam> > ().release())); }
#line 1307 "src/yacc/Bison.cpp"
    break;

  case 37: // FuncParamList: FuncParamList COMMA FuncParam
#line 180 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<FuncParamList> > () = std::move(yystack_[2].value.as < std::unique_ptr<FuncParamList> > ()); yylhs.value.as < std::unique_ptr<FuncParamList> > ()->pushBack(yystack_[0].value.as < std::unique_ptr<FuncParam> > ().release()); }
#line 1313 "src/yacc/Bison.cpp"
    break;

  case 38: // FuncParam: Type IDENT
#line 185 "src/yacc/sysy.y"
                                            { yylhs.value.as < std::unique_ptr<FuncParam> > () = std::unique_ptr<FuncParam>(new FuncParam(yystack_[1].value.as < ASTType > (), yystack_[0].value.as < LocatedIdentifier > ().text)); }
#line 1319 "src/yacc/Bison.cpp"
    break;

  case 39: // FuncParam: Type IDENT LBRACKET RBRACKET
#line 186 "src/yacc/sysy.y"
                                            { yylhs.value.as < std::unique_ptr<FuncParam> > () = std::unique_ptr<FuncParam>(new FuncParam(yystack_[3].value.as < ASTType > (), yystack_[2].value.as < LocatedIdentifier > ().text, true)); }
#line 1325 "src/yacc/Bison.cpp"
    break;

  case 40: // FuncParam: Type IDENT ExpList
#line 187 "src/yacc/sysy.y"
                                            { yylhs.value.as < std::unique_ptr<FuncParam> > () = std::unique_ptr<FuncParam>(new FuncParam(yystack_[2].value.as < ASTType > (), yystack_[1].value.as < LocatedIdentifier > ().text, false, yystack_[0].value.as < std::unique_ptr<ArrayList> > ().release())); }
#line 1331 "src/yacc/Bison.cpp"
    break;

  case 41: // FuncParam: Type IDENT LBRACKET RBRACKET ExpList
#line 188 "src/yacc/sysy.y"
                                            { yylhs.value.as < std::unique_ptr<FuncParam> > () = std::unique_ptr<FuncParam>(new FuncParam(yystack_[4].value.as < ASTType > (), yystack_[3].value.as < LocatedIdentifier > ().text, true, yystack_[0].value.as < std::unique_ptr<ArrayList> > ().release())); }
#line 1337 "src/yacc/Bison.cpp"
    break;

  case 42: // Block: LBRACE RBRACE
#line 193 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<Block> > () = std::unique_ptr<Block>(new Block(nullptr)); }
#line 1343 "src/yacc/Bison.cpp"
    break;

  case 43: // Block: LBRACE BlockItemList RBRACE
#line 194 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<Block> > () = std::unique_ptr<Block>(new Block(yystack_[1].value.as < std::unique_ptr<BlockItemList> > ().release())); }
#line 1349 "src/yacc/Bison.cpp"
    break;

  case 44: // BlockItemList: BlockItem
#line 198 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<BlockItemList> > () = std::unique_ptr<BlockItemList>(new BlockItemList(yystack_[0].value.as < std::unique_ptr<BaseAST> > ().release())); }
#line 1355 "src/yacc/Bison.cpp"
    break;

  case 45: // BlockItemList: BlockItemList BlockItem
#line 199 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<BlockItemList> > () = std::move(yystack_[1].value.as < std::unique_ptr<BlockItemList> > ()); yylhs.value.as < std::unique_ptr<BlockItemList> > ()->pushBack(yystack_[0].value.as < std::unique_ptr<BaseAST> > ().release()); }
#line 1361 "src/yacc/Bison.cpp"
    break;

  case 46: // BlockItem: Decl
#line 203 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<BaseAST> > () = std::move(yystack_[0].value.as < std::unique_ptr<BaseAST> > ()); }
#line 1367 "src/yacc/Bison.cpp"
    break;

  case 47: // BlockItem: Stmt
#line 204 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<BaseAST> > () = std::move(yystack_[0].value.as < std::unique_ptr<BaseAST> > ()); }
#line 1373 "src/yacc/Bison.cpp"
    break;

  case 48: // Stmt: LVal ASSIGN Exp SEMICOLON
#line 208 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<BaseAST> > () = std::unique_ptr<BaseAST>(new AssignStmt(yystack_[3].value.as < std::unique_ptr<LVal> > ().release(), yystack_[1].value.as < std::unique_ptr<AddExp> > ().release())); }
#line 1379 "src/yacc/Bison.cpp"
    break;

  case 49: // Stmt: SEMICOLON
#line 209 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<BaseAST> > () = std::unique_ptr<BaseAST>(new ExpStmt(nullptr)); }
#line 1385 "src/yacc/Bison.cpp"
    break;

  case 50: // Stmt: Exp SEMICOLON
#line 210 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<BaseAST> > () = std::unique_ptr<BaseAST>(new ExpStmt(yystack_[1].value.as < std::unique_ptr<AddExp> > ().release())); }
#line 1391 "src/yacc/Bison.cpp"
    break;

  case 51: // Stmt: Block
#line 211 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<BaseAST> > () = std::move(yystack_[0].value.as < std::unique_ptr<Block> > ()); }
#line 1397 "src/yacc/Bison.cpp"
    break;

  case 52: // Stmt: IF LPAREN LOrExp RPAREN Stmt
#line 212 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<BaseAST> > () = std::unique_ptr<BaseAST>(new IfStmt(yystack_[2].value.as < std::unique_ptr<LOrExp> > ().release(), yystack_[0].value.as < std::unique_ptr<BaseAST> > ().release())); }
#line 1403 "src/yacc/Bison.cpp"
    break;

  case 53: // Stmt: IF LPAREN LOrExp RPAREN Stmt ELSE Stmt
#line 214 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<BaseAST> > () = std::unique_ptr<BaseAST>(new IfStmt(yystack_[4].value.as < std::unique_ptr<LOrExp> > ().release(), yystack_[2].value.as < std::unique_ptr<BaseAST> > ().release(), yystack_[0].value.as < std::unique_ptr<BaseAST> > ().release())); }
#line 1409 "src/yacc/Bison.cpp"
    break;

  case 54: // Stmt: WHILE LPAREN LOrExp RPAREN Stmt
#line 215 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<BaseAST> > () = std::unique_ptr<BaseAST>(new WhileStmt(yystack_[2].value.as < std::unique_ptr<LOrExp> > ().release(), yystack_[0].value.as < std::unique_ptr<BaseAST> > ().release())); }
#line 1415 "src/yacc/Bison.cpp"
    break;

  case 55: // Stmt: BREAK SEMICOLON
#line 216 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<BaseAST> > () = std::unique_ptr<BaseAST>(new BreakStmt()); }
#line 1421 "src/yacc/Bison.cpp"
    break;

  case 56: // Stmt: CONTINUE SEMICOLON
#line 217 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<BaseAST> > () = std::unique_ptr<BaseAST>(new ContinueStmt()); }
#line 1427 "src/yacc/Bison.cpp"
    break;

  case 57: // Stmt: RETURN SEMICOLON
#line 218 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<BaseAST> > () = std::unique_ptr<BaseAST>(new ReturnStmt()); }
#line 1433 "src/yacc/Bison.cpp"
    break;

  case 58: // Stmt: RETURN Exp SEMICOLON
#line 219 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<BaseAST> > () = std::unique_ptr<BaseAST>(new ReturnStmt(yystack_[1].value.as < std::unique_ptr<AddExp> > ().release())); }
#line 1439 "src/yacc/Bison.cpp"
    break;

  case 59: // PrimaryExp: LPAREN Exp RPAREN
#line 225 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<BaseAST> > () = std::move(yystack_[1].value.as < std::unique_ptr<AddExp> > ()); }
#line 1445 "src/yacc/Bison.cpp"
    break;

  case 60: // PrimaryExp: LVal
#line 226 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<BaseAST> > () = std::move(yystack_[0].value.as < std::unique_ptr<LVal> > ()); }
#line 1451 "src/yacc/Bison.cpp"
    break;

  case 61: // PrimaryExp: INT_CONST
#line 227 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<BaseAST> > () = std::unique_ptr<BaseAST>(new ConValue<int>(yystack_[0].value.as < int > ())); }
#line 1457 "src/yacc/Bison.cpp"
    break;

  case 62: // PrimaryExp: FLOAT_CONST
#line 228 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<BaseAST> > () = std::unique_ptr<BaseAST>(new ConValue<float>(yystack_[0].value.as < float > ())); }
#line 1463 "src/yacc/Bison.cpp"
    break;

  case 63: // PrimaryExp: IDENT LPAREN RPAREN
#line 229 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<BaseAST> > () = std::unique_ptr<BaseAST>(new FuncCall(yystack_[2].value.as < LocatedIdentifier > ().text, yystack_[2].value.as < LocatedIdentifier > ().line)); }
#line 1469 "src/yacc/Bison.cpp"
    break;

  case 64: // PrimaryExp: IDENT LPAREN FuncRParamList RPAREN
#line 230 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<BaseAST> > () = std::unique_ptr<BaseAST>(new FuncCall(yystack_[3].value.as < LocatedIdentifier > ().text, yystack_[1].value.as < std::unique_ptr<FuncRParamList> > ().release(), yystack_[3].value.as < LocatedIdentifier > ().line)); }
#line 1475 "src/yacc/Bison.cpp"
    break;

  case 65: // LVal: IDENT
#line 234 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<LVal> > () = std::unique_ptr<LVal>(new LVal(yystack_[0].value.as < LocatedIdentifier > ().text)); }
#line 1481 "src/yacc/Bison.cpp"
    break;

  case 66: // LVal: IDENT ExpList
#line 235 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<LVal> > () = std::unique_ptr<LVal>(new LVal(yystack_[1].value.as < LocatedIdentifier > ().text, yystack_[0].value.as < std::unique_ptr<ArrayList> > ().release())); }
#line 1487 "src/yacc/Bison.cpp"
    break;

  case 67: // ExpList: LBRACKET Exp RBRACKET
#line 239 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<ArrayList> > () = std::unique_ptr<ArrayList>(new ArrayList(yystack_[1].value.as < std::unique_ptr<AddExp> > ().release())); }
#line 1493 "src/yacc/Bison.cpp"
    break;

  case 68: // ExpList: ExpList LBRACKET Exp RBRACKET
#line 240 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<ArrayList> > () = std::move(yystack_[3].value.as < std::unique_ptr<ArrayList> > ()); yylhs.value.as < std::unique_ptr<ArrayList> > ()->pushBack(yystack_[1].value.as < std::unique_ptr<AddExp> > ().release()); }
#line 1499 "src/yacc/Bison.cpp"
    break;

  case 69: // ConstExpList: LBRACKET ConstExp RBRACKET
#line 244 "src/yacc/sysy.y"
                                                { yylhs.value.as < std::unique_ptr<ArrayList> > () = std::unique_ptr<ArrayList>(new ArrayList(yystack_[1].value.as < std::unique_ptr<AddExp> > ().release())); }
#line 1505 "src/yacc/Bison.cpp"
    break;

  case 70: // ConstExpList: ConstExpList LBRACKET ConstExp RBRACKET
#line 245 "src/yacc/sysy.y"
                                                { yylhs.value.as < std::unique_ptr<ArrayList> > () = std::move(yystack_[3].value.as < std::unique_ptr<ArrayList> > ()); yylhs.value.as < std::unique_ptr<ArrayList> > ()->pushBack(yystack_[1].value.as < std::unique_ptr<AddExp> > ().release()); }
#line 1511 "src/yacc/Bison.cpp"
    break;

  case 71: // FuncRParamList: Exp
#line 249 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<FuncRParamList> > () = std::unique_ptr<FuncRParamList>(new FuncRParamList(yystack_[0].value.as < std::unique_ptr<AddExp> > ().release())); }
#line 1517 "src/yacc/Bison.cpp"
    break;

  case 72: // FuncRParamList: FuncRParamList COMMA Exp
#line 250 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<FuncRParamList> > () = std::move(yystack_[2].value.as < std::unique_ptr<FuncRParamList> > ()); yylhs.value.as < std::unique_ptr<FuncRParamList> > ()->pushBack(yystack_[0].value.as < std::unique_ptr<AddExp> > ().release()); }
#line 1523 "src/yacc/Bison.cpp"
    break;

  case 73: // UnaryExp: PrimaryExp
#line 255 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<UnaryExp> > () = std::unique_ptr<UnaryExp>(new UnaryExp(yystack_[0].value.as < std::unique_ptr<BaseAST> > ().release())); }
#line 1529 "src/yacc/Bison.cpp"
    break;

  case 74: // UnaryExp: ADD UnaryExp
#line 256 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<UnaryExp> > () = std::move(yystack_[0].value.as < std::unique_ptr<UnaryExp> > ()); yylhs.value.as < std::unique_ptr<UnaryExp> > ()->pushFront(SY_ADD); }
#line 1535 "src/yacc/Bison.cpp"
    break;

  case 75: // UnaryExp: SUB UnaryExp
#line 257 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<UnaryExp> > () = std::move(yystack_[0].value.as < std::unique_ptr<UnaryExp> > ()); yylhs.value.as < std::unique_ptr<UnaryExp> > ()->pushFront(SY_SUB); }
#line 1541 "src/yacc/Bison.cpp"
    break;

  case 76: // UnaryExp: NOT UnaryExp
#line 258 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<UnaryExp> > () = std::move(yystack_[0].value.as < std::unique_ptr<UnaryExp> > ()); yylhs.value.as < std::unique_ptr<UnaryExp> > ()->pushFront(SY_NOT); }
#line 1547 "src/yacc/Bison.cpp"
    break;

  case 77: // MulExp: UnaryExp
#line 263 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<MulExp> > () = std::unique_ptr<MulExp>(new MulExp(yystack_[0].value.as < std::unique_ptr<UnaryExp> > ().release())); }
#line 1553 "src/yacc/Bison.cpp"
    break;

  case 78: // MulExp: MulExp MUL UnaryExp
#line 264 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<MulExp> > () = std::move(yystack_[2].value.as < std::unique_ptr<MulExp> > ()); yylhs.value.as < std::unique_ptr<MulExp> > ()->pushBack(SY_MUL); yylhs.value.as < std::unique_ptr<MulExp> > ()->pushBack(yystack_[0].value.as < std::unique_ptr<UnaryExp> > ().release()); }
#line 1559 "src/yacc/Bison.cpp"
    break;

  case 79: // MulExp: MulExp DIV UnaryExp
#line 265 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<MulExp> > () = std::move(yystack_[2].value.as < std::unique_ptr<MulExp> > ()); yylhs.value.as < std::unique_ptr<MulExp> > ()->pushBack(SY_DIV); yylhs.value.as < std::unique_ptr<MulExp> > ()->pushBack(yystack_[0].value.as < std::unique_ptr<UnaryExp> > ().release()); }
#line 1565 "src/yacc/Bison.cpp"
    break;

  case 80: // MulExp: MulExp MOD UnaryExp
#line 266 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<MulExp> > () = std::move(yystack_[2].value.as < std::unique_ptr<MulExp> > ()); yylhs.value.as < std::unique_ptr<MulExp> > ()->pushBack(SY_MOD); yylhs.value.as < std::unique_ptr<MulExp> > ()->pushBack(yystack_[0].value.as < std::unique_ptr<UnaryExp> > ().release()); }
#line 1571 "src/yacc/Bison.cpp"
    break;

  case 81: // AddExp: MulExp
#line 270 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<AddExp> > () = std::unique_ptr<AddExp>(new AddExp(yystack_[0].value.as < std::unique_ptr<MulExp> > ().release())); }
#line 1577 "src/yacc/Bison.cpp"
    break;

  case 82: // AddExp: AddExp ADD MulExp
#line 271 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<AddExp> > () = std::move(yystack_[2].value.as < std::unique_ptr<AddExp> > ()); yylhs.value.as < std::unique_ptr<AddExp> > ()->pushBack(SY_ADD); yylhs.value.as < std::unique_ptr<AddExp> > ()->pushBack(yystack_[0].value.as < std::unique_ptr<MulExp> > ().release()); }
#line 1583 "src/yacc/Bison.cpp"
    break;

  case 83: // AddExp: AddExp SUB MulExp
#line 272 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<AddExp> > () = std::move(yystack_[2].value.as < std::unique_ptr<AddExp> > ()); yylhs.value.as < std::unique_ptr<AddExp> > ()->pushBack(SY_SUB); yylhs.value.as < std::unique_ptr<AddExp> > ()->pushBack(yystack_[0].value.as < std::unique_ptr<MulExp> > ().release()); }
#line 1589 "src/yacc/Bison.cpp"
    break;

  case 84: // RelExp: AddExp
#line 276 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<RelExp> > () = std::unique_ptr<RelExp>(new RelExp(yystack_[0].value.as < std::unique_ptr<AddExp> > ().release())); }
#line 1595 "src/yacc/Bison.cpp"
    break;

  case 85: // RelExp: RelExp LESS AddExp
#line 277 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<RelExp> > () = std::move(yystack_[2].value.as < std::unique_ptr<RelExp> > ()); yylhs.value.as < std::unique_ptr<RelExp> > ()->pushBack(SY_LESS); yylhs.value.as < std::unique_ptr<RelExp> > ()->pushBack(yystack_[0].value.as < std::unique_ptr<AddExp> > ().release()); }
#line 1601 "src/yacc/Bison.cpp"
    break;

  case 86: // RelExp: RelExp GREAT AddExp
#line 278 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<RelExp> > () = std::move(yystack_[2].value.as < std::unique_ptr<RelExp> > ()); yylhs.value.as < std::unique_ptr<RelExp> > ()->pushBack(SY_GREAT); yylhs.value.as < std::unique_ptr<RelExp> > ()->pushBack(yystack_[0].value.as < std::unique_ptr<AddExp> > ().release()); }
#line 1607 "src/yacc/Bison.cpp"
    break;

  case 87: // RelExp: RelExp LESSEQ AddExp
#line 279 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<RelExp> > () = std::move(yystack_[2].value.as < std::unique_ptr<RelExp> > ()); yylhs.value.as < std::unique_ptr<RelExp> > ()->pushBack(SY_LESSEQ); yylhs.value.as < std::unique_ptr<RelExp> > ()->pushBack(yystack_[0].value.as < std::unique_ptr<AddExp> > ().release()); }
#line 1613 "src/yacc/Bison.cpp"
    break;

  case 88: // RelExp: RelExp GREATEQ AddExp
#line 280 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<RelExp> > () = std::move(yystack_[2].value.as < std::unique_ptr<RelExp> > ()); yylhs.value.as < std::unique_ptr<RelExp> > ()->pushBack(SY_GREATEQ); yylhs.value.as < std::unique_ptr<RelExp> > ()->pushBack(yystack_[0].value.as < std::unique_ptr<AddExp> > ().release()); }
#line 1619 "src/yacc/Bison.cpp"
    break;

  case 89: // EqExp: RelExp
#line 284 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<EqExp> > () = std::unique_ptr<EqExp>(new EqExp(yystack_[0].value.as < std::unique_ptr<RelExp> > ().release())); }
#line 1625 "src/yacc/Bison.cpp"
    break;

  case 90: // EqExp: EqExp EQ RelExp
#line 285 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<EqExp> > () = std::move(yystack_[2].value.as < std::unique_ptr<EqExp> > ()); yylhs.value.as < std::unique_ptr<EqExp> > ()->pushBack(SY_EQ); yylhs.value.as < std::unique_ptr<EqExp> > ()->pushBack(yystack_[0].value.as < std::unique_ptr<RelExp> > ().release()); }
#line 1631 "src/yacc/Bison.cpp"
    break;

  case 91: // EqExp: EqExp NOTEQ RelExp
#line 286 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<EqExp> > () = std::move(yystack_[2].value.as < std::unique_ptr<EqExp> > ()); yylhs.value.as < std::unique_ptr<EqExp> > ()->pushBack(SY_NOTEQ); yylhs.value.as < std::unique_ptr<EqExp> > ()->pushBack(yystack_[0].value.as < std::unique_ptr<RelExp> > ().release()); }
#line 1637 "src/yacc/Bison.cpp"
    break;

  case 92: // LAndExp: EqExp
#line 290 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<LAndExp> > () = std::unique_ptr<LAndExp>(new LAndExp(yystack_[0].value.as < std::unique_ptr<EqExp> > ().release())); }
#line 1643 "src/yacc/Bison.cpp"
    break;

  case 93: // LAndExp: LAndExp AND EqExp
#line 291 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<LAndExp> > () = std::move(yystack_[2].value.as < std::unique_ptr<LAndExp> > ()); yylhs.value.as < std::unique_ptr<LAndExp> > ()->pushBack(SY_AND); yylhs.value.as < std::unique_ptr<LAndExp> > ()->pushBack(yystack_[0].value.as < std::unique_ptr<EqExp> > ().release()); }
#line 1649 "src/yacc/Bison.cpp"
    break;

  case 94: // LOrExp: LAndExp
#line 295 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<LOrExp> > () = std::unique_ptr<LOrExp>(new LOrExp(yystack_[0].value.as < std::unique_ptr<LAndExp> > ().release())); }
#line 1655 "src/yacc/Bison.cpp"
    break;

  case 95: // LOrExp: LOrExp OR LAndExp
#line 296 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<LOrExp> > () = std::move(yystack_[2].value.as < std::unique_ptr<LOrExp> > ()); yylhs.value.as < std::unique_ptr<LOrExp> > ()->pushBack(SY_OR); yylhs.value.as < std::unique_ptr<LOrExp> > ()->pushBack(yystack_[0].value.as < std::unique_ptr<LAndExp> > ().release()); }
#line 1661 "src/yacc/Bison.cpp"
    break;

  case 96: // Exp: AddExp
#line 300 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<AddExp> > () = std::move(yystack_[0].value.as < std::unique_ptr<AddExp> > ()); }
#line 1667 "src/yacc/Bison.cpp"
    break;

  case 97: // ConstExp: AddExp
#line 304 "src/yacc/sysy.y"
                                        { yylhs.value.as < std::unique_ptr<AddExp> > () = std::move(yystack_[0].value.as < std::unique_ptr<AddExp> > ()); }
#line 1673 "src/yacc/Bison.cpp"
    break;


#line 1677 "src/yacc/Bison.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (YY_MOVE (msg));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // parser::context.
  parser::context::context (const parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short parser::yypact_ninf_ = -141;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
     156,    84,  -141,  -141,  -141,    24,   156,    47,  -141,  -141,
    -141,  -141,    50,  -141,  -141,  -141,    -7,    83,  -141,    -2,
      97,  -141,    71,   252,   212,     3,  -141,    55,   216,     5,
    -141,    50,    65,   106,    41,  -141,  -141,  -141,    61,   252,
     252,   252,   252,  -141,  -141,  -141,    87,    94,     8,    35,
    -141,    94,  -141,   252,   212,    37,  -141,    68,  -141,  -141,
     216,  -141,   135,  -141,    95,    65,    84,   236,   252,   116,
     100,  -141,  -141,  -141,   252,   252,   252,   252,   252,  -141,
    -141,  -141,    66,   126,  -141,  -141,  -141,    93,  -141,   232,
     136,   144,   157,   162,  -141,  -141,    55,  -141,  -141,   167,
    -141,  -141,   149,   166,    92,   116,  -141,  -141,  -141,    42,
    -141,   169,   252,  -141,  -141,  -141,  -141,    87,    87,  -141,
     212,  -141,  -141,   216,  -141,   170,   252,   252,  -141,  -141,
    -141,  -141,   252,  -141,   171,  -141,   252,  -141,   173,  -141,
    -141,  -141,    94,   242,   128,   174,    11,    39,   176,   116,
    -141,  -141,   252,   252,   252,   252,   252,   252,   252,   192,
     252,   192,  -141,    94,    94,    94,    94,   242,   242,   128,
     197,   174,  -141,   192,  -141
  };

  const signed char
  parser::yydefact_[] =
  {
       0,     0,     7,     8,     9,     0,     2,     0,     4,    10,
      11,     3,     0,     1,     6,     5,    20,     0,    18,     0,
       0,    13,     0,     0,     0,    22,    17,     0,     0,     0,
      12,     0,     0,     0,     0,    36,    61,    62,    65,     0,
       0,     0,     0,    73,    60,    77,    81,    97,     0,     0,
      21,    96,    29,     0,     0,    20,    19,     0,    15,    24,
       0,    14,     0,    34,    38,     0,     0,     0,     0,    66,
       0,    76,    74,    75,     0,     0,     0,     0,     0,    69,
      30,    32,     0,     0,    23,    25,    27,     0,    16,     0,
       0,     0,     0,     0,    42,    49,     0,    46,    51,     0,
      44,    47,    60,     0,     0,    40,    35,    37,    63,     0,
      71,     0,     0,    59,    78,    79,    80,    82,    83,    31,
       0,    70,    26,     0,    57,     0,     0,     0,    55,    56,
      43,    45,     0,    50,    39,    64,     0,    67,     0,    33,
      28,    58,    84,    89,    92,    94,     0,     0,     0,    41,
      72,    68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    85,    86,    87,    88,    90,    91,    93,
      52,    95,    54,     0,    53
  };

  const short
  parser::yypgoto_[] =
  {
    -141,  -141,  -141,     0,    20,  -141,  -141,   180,  -141,  -141,
     177,   -53,  -141,   -46,  -141,   206,  -141,   147,   -21,  -141,
     123,  -140,  -141,   -52,   -58,   204,  -141,   -26,    81,   -23,
      10,    69,    64,   102,   -22,   -11
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,     5,     6,     7,    97,     9,    20,    21,    10,    17,
      18,    58,    87,    50,    82,    11,    34,    35,    98,    99,
     100,   101,    43,    44,    69,    25,   109,    45,    46,    51,
     143,   144,   145,   146,   103,    59
  };

  const unsigned char
  parser::yytable_[] =
  {
      47,    12,    52,    81,    86,    47,   105,    88,    84,    22,
     102,    63,    48,    23,    71,    72,    73,    70,    23,   170,
       8,   172,    33,    53,    13,    53,    14,    52,   159,    79,
      47,    24,    52,   174,    47,   160,    28,    47,    36,    37,
      38,    54,    83,    60,   106,   110,   111,   102,   114,   115,
     116,    39,    16,    49,    80,    19,   161,    23,    65,   135,
      55,    40,    96,   160,    66,   136,    33,   125,    41,    42,
     140,    36,    37,    38,   139,    24,   149,    67,     2,     3,
       4,    68,   111,    62,    39,   119,    57,    85,    32,   120,
     138,     2,     3,     4,    40,    36,    37,    38,    52,    96,
      47,    41,    42,   142,   142,    26,    27,   102,    39,   102,
     148,    64,   122,   134,   150,   104,   123,   113,    40,    30,
      31,   102,    74,    75,    76,    41,    42,    77,    78,   163,
     164,   165,   166,   142,   142,   142,   112,   142,    36,    37,
      38,     1,     2,     3,     4,    89,    90,   121,    91,    92,
      93,    39,   126,    62,    94,   156,   157,    95,   117,   118,
     127,    40,     1,     2,     3,     4,   167,   168,    41,    42,
      36,    37,    38,     1,     2,     3,     4,    89,    90,   128,
      91,    92,    93,    39,   129,    62,   130,   132,   133,    95,
     137,    68,   141,    40,   151,    36,    37,    38,   162,   158,
      41,    42,    89,    90,    56,    91,    92,    93,    39,   173,
      62,    61,    15,   107,    95,    36,    37,    38,    40,    36,
      37,    38,   131,    29,   171,    41,    42,   169,    39,   147,
      49,     0,    39,     0,    57,    36,    37,    38,    40,    36,
      37,    38,    40,     0,     0,    41,    42,     0,    39,    41,
      42,     0,    39,   108,   124,    36,    37,    38,    40,     0,
       0,     0,    40,     0,     0,    41,    42,     0,    39,    41,
      42,   152,   153,   154,   155,     0,     0,     0,    40,     0,
       0,     0,     0,     0,     0,    41,    42
  };

  const short
  parser::yycheck_[] =
  {
      23,     1,    24,    49,    57,    28,    64,    60,    54,    16,
      62,    32,    23,    20,    40,    41,    42,    39,    20,   159,
       0,   161,    22,    20,     0,    20,     6,    49,    17,    21,
      53,    38,    54,   173,    57,    24,    38,    60,     3,     4,
       5,    38,    53,    38,    65,    67,    68,    99,    74,    75,
      76,    16,     5,    18,    19,     5,    17,    20,    17,    17,
       5,    26,    62,    24,    23,    23,    66,    89,    33,    34,
     123,     3,     4,     5,   120,    38,   134,    16,     7,     8,
       9,    20,   104,    18,    16,    19,    18,    19,    17,    23,
     112,     7,     8,     9,    26,     3,     4,     5,   120,    99,
     123,    33,    34,   126,   127,    22,    23,   159,    16,   161,
     132,     5,    19,    21,   136,    20,    23,    17,    26,    22,
      23,   173,    35,    36,    37,    33,    34,    33,    34,   152,
     153,   154,   155,   156,   157,   158,    20,   160,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    21,    13,    14,
      15,    16,    16,    18,    19,    27,    28,    22,    77,    78,
      16,    26,     6,     7,     8,     9,   156,   157,    33,    34,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    22,
      13,    14,    15,    16,    22,    18,    19,    38,    22,    22,
      21,    20,    22,    26,    21,     3,     4,     5,    22,    25,
      33,    34,    10,    11,    27,    13,    14,    15,    16,    12,
      18,    31,     6,    66,    22,     3,     4,     5,    26,     3,
       4,     5,    99,    19,   160,    33,    34,   158,    16,   127,
      18,    -1,    16,    -1,    18,     3,     4,     5,    26,     3,
       4,     5,    26,    -1,    -1,    33,    34,    -1,    16,    33,
      34,    -1,    16,    17,    22,     3,     4,     5,    26,    -1,
      -1,    -1,    26,    -1,    -1,    33,    34,    -1,    16,    33,
      34,    29,    30,    31,    32,    -1,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    34
  };

  const signed char
  parser::yystos_[] =
  {
       0,     6,     7,     8,     9,    40,    41,    42,    43,    44,
      47,    54,    42,     0,    43,    54,     5,    48,    49,     5,
      45,    46,    16,    20,    38,    64,    22,    23,    38,    64,
      22,    23,    17,    42,    55,    56,     3,     4,     5,    16,
      26,    33,    34,    61,    62,    66,    67,    68,    74,    18,
      52,    68,    73,    20,    38,     5,    49,    18,    50,    74,
      38,    46,    18,    57,     5,    17,    23,    16,    20,    63,
      73,    66,    66,    66,    35,    36,    37,    33,    34,    21,
      19,    52,    53,    74,    52,    19,    50,    51,    50,    10,
      11,    13,    14,    15,    19,    22,    42,    43,    57,    58,
      59,    60,    62,    73,    20,    63,    57,    56,    17,    65,
      73,    73,    20,    17,    66,    66,    66,    67,    67,    19,
      23,    21,    19,    23,    22,    73,    16,    16,    22,    22,
      19,    59,    38,    22,    21,    17,    23,    21,    73,    52,
      50,    22,    68,    69,    70,    71,    72,    72,    73,    63,
      73,    21,    29,    30,    31,    32,    27,    28,    25,    17,
      24,    17,    22,    68,    68,    68,    68,    69,    69,    70,
      60,    71,    60,    12,    60
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    39,    40,    41,    41,    41,    41,    42,    42,    42,
      43,    43,    44,    45,    45,    46,    46,    47,    48,    48,
      49,    49,    49,    49,    50,    50,    50,    51,    51,    52,
      52,    52,    53,    53,    54,    54,    55,    55,    56,    56,
      56,    56,    57,    57,    58,    58,    59,    59,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    61,
      61,    61,    61,    61,    61,    62,    62,    63,    63,    64,
      64,    65,    65,    66,    66,    66,    66,    67,    67,    67,
      67,    68,    68,    68,    69,    69,    69,    69,    69,    70,
      70,    70,    71,    71,    72,    72,    73,    74
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     1,     4,     1,     3,     3,     4,     3,     1,     3,
       1,     3,     2,     4,     1,     2,     3,     1,     3,     1,
       2,     3,     1,     3,     5,     6,     1,     3,     2,     4,
       3,     5,     2,     3,     1,     2,     1,     1,     4,     1,
       2,     1,     5,     7,     5,     2,     2,     2,     3,     3,
       1,     1,     1,     3,     4,     1,     2,     3,     4,     3,
       4,     1,     3,     1,     2,     2,     2,     1,     3,     3,
       3,     1,     3,     3,     1,     3,     3,     3,     3,     1,
       3,     3,     1,     3,     1,     3,     1,     1
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "INT_CONST",
  "FLOAT_CONST", "IDENT", "CONST", "INT", "FLOAT", "VOID", "RETURN", "IF",
  "ELSE", "WHILE", "BREAK", "CONTINUE", "LPAREN", "RPAREN", "LBRACE",
  "RBRACE", "LBRACKET", "RBRACKET", "SEMICOLON", "COMMA", "OR", "AND",
  "NOT", "EQ", "NOTEQ", "LESS", "GREAT", "LESSEQ", "GREATEQ", "ADD", "SUB",
  "MUL", "DIV", "MOD", "ASSIGN", "$accept", "Start", "CompUnit", "Type",
  "Decl", "ConstDecl", "ConstDefList", "ConstDef", "VarDecl", "VarDefList",
  "VarDef", "ConstInitVal", "ConstInitValList", "InitVal", "InitValList",
  "FuncDef", "FuncParamList", "FuncParam", "Block", "BlockItemList",
  "BlockItem", "Stmt", "PrimaryExp", "LVal", "ExpList", "ConstExpList",
  "FuncRParamList", "UnaryExp", "MulExp", "AddExp", "RelExp", "EqExp",
  "LAndExp", "LOrExp", "Exp", "ConstExp", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,    89,    89,    94,    95,    96,    97,   101,   102,   103,
     108,   109,   113,   118,   119,   124,   126,   131,   136,   137,
     141,   142,   143,   144,   149,   150,   151,   155,   156,   160,
     161,   162,   166,   167,   172,   174,   179,   180,   185,   186,
     187,   188,   193,   194,   198,   199,   203,   204,   208,   209,
     210,   211,   212,   213,   215,   216,   217,   218,   219,   225,
     226,   227,   228,   229,   230,   234,   235,   239,   240,   244,
     245,   249,   250,   255,   256,   257,   258,   263,   264,   265,
     266,   270,   271,   272,   276,   277,   278,   279,   280,   284,
     285,   286,   290,   291,   295,   296,   300,   304
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


} // yy
#line 2282 "src/yacc/Bison.cpp"

#line 307 "src/yacc/sysy.y"

