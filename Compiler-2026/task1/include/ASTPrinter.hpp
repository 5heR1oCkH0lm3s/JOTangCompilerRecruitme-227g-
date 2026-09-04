#pragma once

#include "lib/AST.hpp"

#include <ostream>

// 将 AST 输出为稳定的单行 S-expression，供人工观察和自动测试使用。
class ASTPrinter final : public ASTVisitor {
public:
    explicit ASTPrinter(std::ostream& output);

    void print(CompUnit& root);

    void visit(CompUnit& node) override;
    void visit(ConstDecl& node) override;
    void visit(VarDecl& node) override;
    void visit(ConstDefList& node) override;
    void visit(VarDefList& node) override;
    void visit(ConstDef& node) override;
    void visit(VarDef& node) override;
    void visit(FuncDef& node) override;
    void visit(FuncParam& node) override;
    void visit(FuncParamList& node) override;
    void visit(InitValList& node) override;
    void visit(InitVal& node) override;
    void visit(Block& node) override;
    void visit(BlockItemList& node) override;
    void visit(AssignStmt& node) override;
    void visit(ExpStmt& node) override;
    void visit(IfStmt& node) override;
    void visit(WhileStmt& node) override;
    void visit(BreakStmt& node) override;
    void visit(ContinueStmt& node) override;
    void visit(ReturnStmt& node) override;
    void visit(LVal& node) override;
    void visit(FuncCall& node) override;
    void visit(ArrayList& node) override;
    void visit(FuncRParamList& node) override;
    void visit(UnaryExp& node) override;
    void visit(MulExp& node) override;
    void visit(AddExp& node) override;
    void visit(RelExp& node) override;
    void visit(EqExp& node) override;
    void visit(LAndExp& node) override;
    void visit(LOrExp& node) override;
    void visit(ConValue<int>& node) override;
    void visit(ConValue<float>& node) override;

private:
    std::ostream& output;

    void printChild(BaseAST* child);
    void printType(const ASTType& type);
    void printOperator(Type operation);

    template <typename OperandT>
    void printExpression(const char* name, BaseExp<OperandT>& node) {
        output << '(' << name << " (Ops";
        for (Type operation : node.getOps()) {
            output << ' ';
            printOperator(operation);
        }
        output << ')';
        for (const auto& operand : node.getOperands()) {
            output << ' ';
            printChild(operand.get());
        }
        output << ')';
    }
};
