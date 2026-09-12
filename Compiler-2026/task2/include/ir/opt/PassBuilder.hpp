#pragma once
#include "ir/opt/NewPassManager.hpp"

//O0输出前端IR；O1和O2均运行SSA、循环规范化及学生实现的优化pass。
enum class OptimizationLevel {
    O0 = 0,
    O1 = 1,
    O2 = 2
};

//组织基础流水线，并为后续学生实现的Pass保留统一入口。
class PassBuilder {
public:
    explicit PassBuilder();
    //只注册支配树和循环分析；由变换pass按需获取结果。
    void registerFunctionAnalyses(FunctionAnalysisPassManager &FAM);
    //构造Mem2Reg、LoopSimplify、LCSSA及LocalGVN、LICM组成的教学流水线。
    ModulePassManager buildModulePipeline(
        OptimizationLevel Level, FunctionAnalysisPassManager &FAM);
};
