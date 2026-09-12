#include "ir/opt/PassBuilder.hpp"
#include "ir/analysis/Dominant.hpp"
#include "ir/analysis/LoopInfo.hpp"
#include "ir/opt/LoopSimplify.hpp"
#include "ir/opt/LCSSA.hpp"
#include "ir/opt/Mem2Reg.hpp"
#include "ir/opt/LocalGVN.hpp"
#include "ir/opt/LICM.hpp"
#include <utility>

PassBuilder::PassBuilder() = default;

void PassBuilder::registerFunctionAnalyses(
    FunctionAnalysisPassManager &FAM) {
    #define FUNCTION_ANALYSIS(NAME, CREATE_PASS) \
        FAM.registerPass([] { return CREATE_PASS; });
    #include "FunctionAnalysisPass.def"
    #undef FUNCTION_ANALYSIS
}

ModulePassManager PassBuilder::buildModulePipeline(
    OptimizationLevel Level, FunctionAnalysisPassManager &FAM) {
    ModulePassManager MPM;
    MPM.setIRMutationCallback([&FAM] { FAM.clear(); });
    if (Level == OptimizationLevel::O0) return MPM;

    FunctionPassManager FPM;
    //先把可提升的局部变量转为SSA，保留必要的合流与循环PHI。
    FPM.addPass(PromotePass());
    //规范化到稳定状态：唯一预头、唯一回边锁存块和专用出口。
    FPM.addPass(LoopSimplifyPass());
    //通过出口PHI封闭循环内定义的外部使用。
    FPM.addPass(LCSSAPass());
    //两项学生实现任务：先清理块内重复计算，再外提循环不变量。
    FPM.addPass(LocalGVNPass());
    FPM.addPass(LICMPass());
    //外提可能把不同循环块的重复计算集中到同一个preheader。
    FPM.addPass(LocalGVNPass());
    MPM.addPass(createModuleFunctionBridge(std::move(FPM), FAM));
    return MPM;
}
