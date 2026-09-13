#pragma once

#include "ir/opt/NewPassManager.hpp"

//任务：把可安全提前执行的循环不变整数纯计算外提到preheader。
//输入由Mem2Reg、LoopSimplify和LCSSA规范化，基础实现只需支配树和LoopInfo。
//基础范围：整数加减乘、按位与/或/异或、整数比较；处理不变量链及嵌套循环。
class LICMPass : public PassDescriptor<LICMPass> {
public:
    PassResult run(Function &F, FunctionAnalysisPassManager &FAM);

private:

};
