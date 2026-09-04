#pragma once

#include "lib/AST.hpp"

#include <memory>

// 解析完整个编译单元后，Bison 的开始符号语义动作应设置此根节点。
extern std::unique_ptr<CompUnit> ASTRoot;
