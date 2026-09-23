#include "ir/opt/LICM.hpp"
#include "ir/analysis/Dominant.hpp"
#include "ir/analysis/LoopInfo.hpp"
#include <utility>
#include <vector>

namespace {

// 可以外提的指令：整数加减乘、按位与/或/异或、整数比较。
// 判断依据有两条，缺一不可：
//   1) 无副作用、不会陷入 —— 因此把它提前到循环外“无条件执行”始终安全，
//      即使循环一次都不执行，也不会改变程序行为（这是 speculation 成立的前提）；
//   2) 结果只依赖操作数 —— 没有隐藏的运行时状态。
// 除法/取模可能因除零而陷入，load/store 有内存副作用，call 可能改变全局状态，
// 它们都不能无条件提前执行，因此不在基础范围内。
bool IsHoistable(const Instruction *I) {
    switch (I->instType) {
    case Instruction::ADD:
    case Instruction::SUB:
    case Instruction::MUL:
    case Instruction::AND:
    case Instruction::OR:
    case Instruction::XOR:
    case Instruction::ICMP:
        return true;
    default:
        return false;
    }
}

// 一条指令成为循环不变量的条件：它的每一个操作数本身都是循环不变值。
// 操作数是常量、函数参数、全局变量，或定义在循环之外的指令时，
// LoopInfo::isLoopInvariant 都返回 true。
bool AllOperandsInvariant(Instruction *I, const Loop *L) {
    for (std::size_t Index = 0; Index < I->getNumOperands(); ++Index) {
        if (!LoopInfo::isLoopInvariant(I->getOperand(Index), L)) return false;
    }
    return true;
}

} // namespace

PassResult LICMPass::run(Function &F, FunctionAnalysisPassManager &FAM) {
    // 循环结构决定“哪些块属于这个循环”，直接来源是 LoopInfo；
    // 而 LoopInfo 本身由支配树构建（回边 = 循环头支配其前驱的那条边），
    // 所以这里有向分析管理器取 LoopInfo，等价于隐含地依赖了 DominantAnalysis。
    // 至于“搬到 preheader 后是否仍支配原来所有使用点”，由规范化形态保证：
    // LoopSimplify 已经确保 preheader 是循环外唯一入口、且只通向循环头。
    LoopInfo &LI = FAM.getResult<LoopAnalysis>(F);

    bool Changed = false;
    // 后序遍历 = 内层循环先处理：内层的不变量被提到内层 preheader 后，
    // 这些值位于外层循环体内、但可能已经成为外层循环的不变量，
    // 于是可以在处理外层循环时继续往外提。
    for (Loop *L : LI.getLoopsInPostOrder()) {
        if (!L || !L->getHeader()) continue;

        // 规范形态保证存在唯一的 preheader：循环外、且必然执行的安放点。
        // 拿不到说明该循环还没被 LoopSimplify 规范化，保守跳过。
        BasicBlock *PreHeader = nullptr;
        BasicBlock *Latch = nullptr;
        if (!L->getLoopSimplifyForm(PreHeader, Latch) || !PreHeader) continue;

        // 反复扫描直到本轮不再有新指令被外提：
        // 一条指令外提后，以它为操作数的指令可能从“不是不变量”变成“是不变量”，
        // 这种不变量链需要多轮才能收敛。
        bool LoopChanged = true;
        while (LoopChanged) {
            LoopChanged = false;
            // 按 RPO 遍历本层循环块：定义块一定先于使用块被访问，
            // 保证级联外提时被依赖的值先进入 preheader。
            for (BasicBlock *BB : L->getBlocksInRPO()) {
                if (!BB) continue;

                // 快照本块指令：外提会把指令移出本块，不能边遍历边改链表。
                std::vector<Instruction *> Instructions;
                for (Instruction *I : *BB) {
                    if (I) Instructions.push_back(I);
                }

                for (Instruction *I : Instructions) {
                    if (!IsHoistable(I)) continue;
                    // 已经在本轮前被外提的指令不再属于循环，跳过。
                    if (!L->contains(I->getParent())) continue;
                    if (!AllOperandsInvariant(I, L)) continue;

                    // 从原块摘除后插入 preheader 的终结指令之前。
                    // 逐条追加可保持扫描顺序，从而保持“先定义后使用”。
                    I->eraseFromParent();
                    PreHeader->insertBeforeTerminator(I);
                    LoopChanged = true;
                    Changed = true;
                }
            }
        }
    }

    if (!Changed) return PassResult::unchanged();

    // 只搬移指令、不修改 CFG 的块与边，支配树和循环信息仍然有效。
    // （分析结果按查询即时计算，不缓存指令级支配，因此搬移不会让它们失真。）
    PreservationStatus PA;
    PA.keep<DominantAnalysis>();
    PA.keep<LoopAnalysis>();
    return PassResult::changed(std::move(PA));
}
