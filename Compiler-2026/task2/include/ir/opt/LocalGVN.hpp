#pragma once

#include "ir/opt/NewPassManager.hpp"

//教学任务：基于局部值编号，消除单个基本块内的重复整数纯计算。
//基础实现利用Mem2Reg生成的SSA和Use/User接口，不需要额外分析pass。
//基础范围：整数加减乘、按位与/或/异或、整数比较；支持交换律和级联重复。
class LocalGVNPass : public PassDescriptor<LocalGVNPass> {
public:
    PassResult run(Function &F, FunctionAnalysisPassManager &FAM);

private:
};
