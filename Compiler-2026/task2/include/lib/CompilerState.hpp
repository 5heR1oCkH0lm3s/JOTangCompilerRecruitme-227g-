#pragma once

#include "lib/AST.hpp"
#include "lib/CFG.hpp"
#include <cstdio>
#include <memory>

//前端与IR生成共享的编译单元状态。
inline std::unique_ptr<CompUnit> ASTRoot = nullptr;
inline std::unique_ptr<Module> ModuleIR = std::make_unique<Module>();

//词法分析器提供的输入文件接口。
extern FILE *yyin;
