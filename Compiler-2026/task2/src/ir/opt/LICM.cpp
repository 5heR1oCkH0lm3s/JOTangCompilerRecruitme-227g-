#include "ir/opt/LICM.hpp"

PassResult LICMPass::run(Function &F, FunctionAnalysisPassManager &FAM) {
    (void)F;
    (void)FAM;

    //空框架不修改IR；完成算法后应替换此返回逻辑。
    return PassResult::unchanged();
}
