#pragma once

#include "ir/opt/NewPassManager.hpp"

//教学任务：基于局部值编号，消除单个基本块内的重复整数纯计算。
//基础实现利用Mem2Reg生成的SSA和Use/User接口，不需要额外分析pass。
//基础范围：整数加减乘、按位与/或/异或、整数比较；支持交换律和级联重复。
//不处理浮点、除余、移位、访存、调用、GEP、转换、select、PHI及undef操作数。
//不进行跨块复用、常量折叠或代数恒等式化简，保持CFG和LCSSA形式。
//LocalGVN.cpp中的进阶TODO是选做方向，不属于上述基础范围。
class LocalGVNPass : public PassDescriptor<LocalGVNPass> {
public:
    PassResult run(Function &F, FunctionAnalysisPassManager &FAM);

private:
    //TODO(LocalGVN-1)：自行设计表达式键、相等比较、哈希及必要的辅助接口。
    //类型定义和接口声明放在本头文件，实现放在LocalGVN.cpp。
};
