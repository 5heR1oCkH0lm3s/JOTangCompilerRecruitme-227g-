#include "./include/yacc/Bison.hpp"
#include "./include/lib/IRGenerator.hpp"
#include "./include/ir/opt/PassBuilder.hpp"
#include "lib/CompilerState.hpp"
#include <iostream>
#include <string>
#include <memory>
#include <cstdio>
#include <fstream>
#include <filesystem>
#include <exception>

int main(int argc, const char *argv[]) {
    // 命令格式：compiler_middle_end input.sy -S -o output.ll [-O0|-O1|-O2]
    // 不指定 -o 时，默认输出与输入同目录、同名的 .ll 文件。
    std::string inputPath;
    std::string outputPath;
    OptimizationLevel OptLevel = OptimizationLevel::O1;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            std::cout << "用法：compiler_middle_end input.sy [-S] [-o output.ll] [-O0|-O1|-O2]\n";
            std::cout << "-O0：原始 IR；-O1/-O2：Mem2Reg 与循环规范化（默认 -O1）。\n";
            return 0;
        }
        if (arg == "-S") continue;
        if (arg == "-o") {
            if (i + 1 == argc || !outputPath.empty()) {
                std::cerr << "-o 需要指定一个输出文件\n";
                return 2;
            }
            outputPath = argv[++i];
        } else if (arg == "-O0") {
            OptLevel = OptimizationLevel::O0;
        } else if (arg == "-O1") {
            OptLevel = OptimizationLevel::O1;
        } else if (arg == "-O2") {
            OptLevel = OptimizationLevel::O2;
        } else if (arg.empty() || arg[0] == '-' || !inputPath.empty()) {
            std::cerr << "无效参数：" << arg << '\n';
            return 2;
        } else {
            inputPath = arg;
        }
    }
    if (inputPath.empty()) {
        std::cerr << "用法：compiler_middle_end input.sy [-S] [-o output.ll] [-O0|-O1|-O2]\n";
        return 2;
    }
    if (outputPath.empty()) {
        outputPath = std::filesystem::path(inputPath).replace_extension(".ll").string();
    }
    std::error_code error;
    if (std::filesystem::equivalent(inputPath, outputPath, error)) {
        std::cerr << "输入和输出不能是同一个文件\n";
        return 2;
    }

    // 读取源文件：词法分析 -> 语法分析 -> AST。
    yyin = fopen(inputPath.c_str(), "r");
    if (!yyin) {
        std::cerr << "无法打开源文件：" << inputPath << '\n';
        return 1;
    }
    int parseResult = 1;
    try {
        yy::parser parser;
        parseResult = parser.parse();
    } catch (const std::exception &error) {
        std::cerr << "词法或语法分析异常：" << error.what() << '\n';
    }
    fclose(yyin);
    yyin = nullptr;
    if (parseResult != 0 || !ASTRoot) {
        std::cerr << "编译失败：未能生成有效的语法树\n";
        return 1;
    }

    // IR 生成。
    try {
        IRGenerator irGenerator;
        irGenerator.visit(*ASTRoot);
    } catch (const std::exception &error) {
        std::cerr << "IR 生成失败：" << error.what() << '\n';
        return 1;
    }

    // 注册支配树和循环分析，运行SSA、循环规范化及两项学生优化pass。
    ModuleAnalysisPassManager MAM;
    FunctionAnalysisPassManager FAM;
    PassBuilder PB;
    PB.registerFunctionAnalyses(FAM);
    try {
        ModulePassManager MPM = PB.buildModulePipeline(OptLevel, FAM);
        MPM.run(*ModuleIR, MAM);
    } catch (const std::exception &error) {
        std::cerr << "中端优化失败：" << error.what() << '\n';
        return 1;
    }

    // 输出运行库声明和当前等级的IR，不调用机器码后端。
    std::ofstream llvmOut(outputPath);
    if (!llvmOut.is_open()) {
        std::cerr << "无法打开输出文件：" << outputPath << '\n';
        return 1;
    }
    std::streambuf *oldBuffer = std::cout.rdbuf(llvmOut.rdbuf());
    try {
        BuildInFunction::dumpDefaultDeclarations();
        ModuleIR->dumpIR();
    } catch (const std::exception &error) {
        std::cout.rdbuf(oldBuffer);
        std::cerr << "IR 输出失败：" << error.what() << '\n';
        return 1;
    }
    std::cout.rdbuf(oldBuffer);
    llvmOut.close();
    if (!llvmOut) {
        std::cerr << "写入输出文件失败：" << outputPath << '\n';
        return 1;
    }
    return 0;
}
