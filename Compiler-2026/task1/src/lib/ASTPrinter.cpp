#include "ASTPrinter.hpp"

#include <iomanip>

ASTPrinter::ASTPrinter(std::ostream& output) : output(output) {}

void ASTPrinter::print(CompUnit& root) {
    root.accept(*this);
    output << '\n';
}

void ASTPrinter::printChild(BaseAST* child) {
    if (child == nullptr) {
        output << "(None)";
        return;
    }
    child->accept(*this);
}

void ASTPrinter::printType(const ASTType& type) {
    if (!type.isValid()) {
        output << "invalid";
        return;
    }
    switch (type.getScalarKind()) {
    case ASTScalarKind::Int32:
        output << "int";
        break;
    case ASTScalarKind::Float32:
        output << "float";
        break;
    case ASTScalarKind::Void:
        output << "void";
        break;
    case ASTScalarKind::Invalid:
        output << "invalid";
        break;
    }
}

void ASTPrinter::printOperator(Type operation) {
    switch (operation) {
    case SY_ADD: output << "+"; break;
    case SY_SUB: output << "-"; break;
    case SY_MUL: output << "*"; break;
    case SY_DIV: output << "/"; break;
    case SY_MOD: output << "%"; break;
    case SY_GEMM: output << "@"; break;
    case SY_GREAT: output << ">"; break;
    case SY_GREATEQ: output << ">="; break;
    case SY_LESS: output << "<"; break;
    case SY_LESSEQ: output << "<="; break;
    case SY_EQ: output << "=="; break;
    case SY_NOTEQ: output << "!="; break;
    case SY_OR: output << "||"; break;
    case SY_AND: output << "&&"; break;
    case SY_NOT: output << "!"; break;
    case SY_ASSIGN: output << "="; break;
    case SY_INT: output << "type-int"; break;
    case SY_FLOAT: output << "type-float"; break;
    case SY_VOID: output << "type-void"; break;
    }
}

void ASTPrinter::visit(CompUnit& node) {
    output << "(CompUnit";
    for (const auto& item : node.getItems()) {
        output << ' ';
        printChild(item.get());
    }
    output << ')';
}

void ASTPrinter::visit(ConstDecl& node) {
    output << "(ConstDecl ";
    printType(node.getType());
    output << ' ';
    printChild(node.getConstDefList().get());
    output << ')';
}

void ASTPrinter::visit(VarDecl& node) {
    output << "(VarDecl ";
    printType(node.getType());
    output << ' ';
    printChild(node.getVarDefList().get());
    output << ')';
}

void ASTPrinter::visit(ConstDefList& node) {
    output << "(ConstDefList";
    for (const auto& definition : node.getDefs()) {
        output << ' ';
        printChild(definition.get());
    }
    output << ')';
}

void ASTPrinter::visit(VarDefList& node) {
    output << "(VarDefList";
    for (const auto& definition : node.getDefs()) {
        output << ' ';
        printChild(definition.get());
    }
    output << ')';
}

void ASTPrinter::visit(ConstDef& node) {
    output << "(ConstDef " << std::quoted(node.getIdent()) << ' ';
    printChild(node.getArray());
    output << ' ';
    printChild(node.getInitVal());
    output << ')';
}

void ASTPrinter::visit(VarDef& node) {
    output << "(VarDef " << std::quoted(node.getIdent()) << ' ';
    printChild(node.getArray());
    output << ' ';
    printChild(node.getInitVal());
    output << ')';
}

void ASTPrinter::visit(FuncDef& node) {
    output << "(FuncDef ";
    printType(node.getType());
    output << ' ' << std::quoted(node.getIdent()) << ' ';
    printChild(node.getParams().get());
    output << ' ';
    printChild(node.getFuncBody().get());
    output << ')';
}

void ASTPrinter::visit(FuncParam& node) {
    output << "(FuncParam ";
    printType(node.getType());
    output << ' ' << std::quoted(node.getIdent()) << ' '
           << (node.hasEmptyBrackets() ? "empty-first-dimension" : "scalar-or-explicit-first-dimension")
           << ' ';
    printChild(node.getExpList().get());
    output << ')';
}

void ASTPrinter::visit(FuncParamList& node) {
    output << "(FuncParamList";
    for (const auto& parameter : node.getParams()) {
        output << ' ';
        printChild(parameter.get());
    }
    output << ')';
}

void ASTPrinter::visit(InitValList& node) {
    output << "(InitValList";
    for (const auto& value : node.getInitVals()) {
        output << ' ';
        printChild(value.get());
    }
    output << ')';
}

void ASTPrinter::visit(InitVal& node) {
    output << "(InitVal";
    if (node.getValue()) {
        output << ' ';
        printChild(node.getValue().get());
    }
    output << ')';
}

void ASTPrinter::visit(Block& node) {
    output << "(Block";
    if (node.getItems()) {
        output << ' ';
        printChild(node.getItems().get());
    }
    output << ')';
}

void ASTPrinter::visit(BlockItemList& node) {
    output << "(BlockItemList";
    for (const auto& item : node.getItems()) {
        output << ' ';
        printChild(item.get());
    }
    output << ')';
}

void ASTPrinter::visit(AssignStmt& node) {
    output << "(AssignStmt ";
    printChild(node.getLVal().get());
    output << ' ';
    printChild(node.getExp().get());
    output << ')';
}

void ASTPrinter::visit(ExpStmt& node) {
    output << "(ExpStmt";
    if (node.getExp()) {
        output << ' ';
        printChild(node.getExp().get());
    }
    output << ')';
}

void ASTPrinter::visit(IfStmt& node) {
    output << "(IfStmt ";
    printChild(node.getCondition().get());
    output << ' ';
    printChild(node.getThenBranch().get());
    output << ' ';
    printChild(node.getElseBranch().get());
    output << ')';
}

void ASTPrinter::visit(WhileStmt& node) {
    output << "(WhileStmt ";
    printChild(node.getCondition().get());
    output << ' ';
    printChild(node.getBody().get());
    output << ')';
}

void ASTPrinter::visit(BreakStmt&) { output << "(BreakStmt)"; }
void ASTPrinter::visit(ContinueStmt&) { output << "(ContinueStmt)"; }

void ASTPrinter::visit(ReturnStmt& node) {
    output << "(ReturnStmt";
    if (node.hasReturnValue()) {
        output << ' ';
        printChild(node.getReturnValue().get());
    }
    output << ')';
}

void ASTPrinter::visit(LVal& node) {
    output << "(LVal " << std::quoted(node.getIdent()) << ' ';
    printChild(node.getArray().get());
    output << ')';
}

void ASTPrinter::visit(FuncCall& node) {
    output << "(FuncCall " << std::quoted(node.getIdent()) << ' '
           << node.getLineNo() << ' ';
    printChild(node.getParams().get());
    output << ')';
}

void ASTPrinter::visit(ArrayList& node) {
    output << "(ArrayList";
    for (const auto& expression : node.getExpressions()) {
        output << ' ';
        printChild(expression.get());
    }
    output << ')';
}

void ASTPrinter::visit(FuncRParamList& node) {
    output << "(FuncRParamList";
    for (const auto& parameter : node.getParams()) {
        output << ' ';
        printChild(parameter.get());
    }
    output << ')';
}

void ASTPrinter::visit(UnaryExp& node) { printExpression("UnaryExp", node); }
void ASTPrinter::visit(MulExp& node) { printExpression("MulExp", node); }
void ASTPrinter::visit(AddExp& node) { printExpression("AddExp", node); }
void ASTPrinter::visit(RelExp& node) { printExpression("RelExp", node); }
void ASTPrinter::visit(EqExp& node) { printExpression("EqExp", node); }
void ASTPrinter::visit(LAndExp& node) { printExpression("LAndExp", node); }
void ASTPrinter::visit(LOrExp& node) { printExpression("LOrExp", node); }

void ASTPrinter::visit(ConValue<int>& node) {
    output << "(Int " << node.getValue() << ')';
}

void ASTPrinter::visit(ConValue<float>& node) {
    const auto flags = output.flags();
    output << "(Float " << std::hexfloat << node.getValue() << ')';
    output.flags(flags);
}
