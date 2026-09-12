#pragma once

#include "ir/opt/NewPassManager.hpp"

//教学任务：把可安全提前执行的循环不变整数纯计算外提到preheader。
//输入由Mem2Reg、LoopSimplify和LCSSA规范化，基础实现只需支配树和LoopInfo。
//基础范围：整数加减乘、按位与/或/异或、整数比较；处理不变量链及嵌套循环。
//不移动浮点、除余、移位、访存、调用、GEP、转换、select、PHI及undef操作数。
//只移动已有指令，保持CFG、SSA和LCSSA；不做下沉、内存提升或循环克隆。
//LICM.cpp中的进阶TODO是选做方向，相关额外分析不随基础版提供。
class LICMPass : public PassDescriptor<LICMPass> {
public:
    PassResult run(Function &F, FunctionAnalysisPassManager &FAM);

private:
    //TODO(LICM-1)：自行设计循环处理、不变量判断、安全性检查等辅助接口。
    //需要的类型定义和接口声明放在本头文件，实现放在LICM.cpp。
};
