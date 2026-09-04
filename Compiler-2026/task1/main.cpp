#include "Bison.hpp"
#include "Frontend.hpp"

#include <cstdio>
#include <exception>
#include <iostream>
#include <memory>
#include <string>

std::unique_ptr<CompUnit> ASTRoot;

// 由 Flex 生成的扫描器提供。
extern FILE* yyin;
extern int yylineno;

namespace {

void printUsage(const char* program) {
    std::cerr << "用法：" << program << " <input.sy>\n";
}

} // namespace

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printUsage(argc > 0 ? argv[0] : "sysy_frontend");
        return 2;
    }

    const std::string inputPath = argv[1];
    using FileHandle = std::unique_ptr<FILE, decltype(&std::fclose)>;
    FileHandle input(std::fopen(inputPath.c_str(), "r"), &std::fclose);
    if (!input) {
        std::cerr << "无法打开输入文件：" << inputPath << '\n';
        return 1;
    }

    ASTRoot.reset();
    yyin = input.get();
    yylineno = 1;

    int parseResult = 1;
    try {
        yy::parser parser;
        parseResult = parser.parse();
    } catch (const std::exception& error) {
        yyin = nullptr;
        std::cerr << "前端分析异常：" << error.what() << '\n';
        return 1;
    } catch (...) {
        yyin = nullptr;
        std::cerr << "前端分析发生未知异常\n";
        return 1;
    }

    yyin = nullptr;
    if (parseResult != 0 || !ASTRoot) {
        std::cerr << "编译失败：没有生成有效的 AST 根节点\n";
        return 1;
    }

    // 下一轮的 ASTPrinter 和自动测试将从 ASTRoot 遍历并校验具体节点。
    std::cout << "AST generated successfully: "
              << ASTRoot->getItems().size()
              << " top-level item(s)\n";
    return 0;
}
