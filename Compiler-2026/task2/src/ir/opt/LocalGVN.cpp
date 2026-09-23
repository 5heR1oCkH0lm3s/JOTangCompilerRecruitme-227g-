#include "ir/opt/LocalGVN.hpp"
#include "ir/analysis/Dominant.hpp"
#include "ir/analysis/LoopInfo.hpp"
#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

namespace {

// 参与块内值编号的指令：整数加减乘、按位与/或/异或、整数比较。
// 这些指令都没有副作用、也不会陷入，因此“算出过一次就一定能复用”。
// 除法/取模（可能陷入）、移位、浮点运算均不在基础范围内。
bool IsValueNumberingCandidate(const Instruction *I) {
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

// 一条候选指令的“值编号键”：操作码 + 两个操作数的代表值。
// 因为 IR 已经是 SSA，操作数的内存地址就唯一代表了它引用的那个值；
// 于是“键相同”等价于“表达式相同”。
struct ExprKey {
    unsigned Op = 0;
    const Value *L = nullptr;
    const Value *R = nullptr;

    bool operator==(const ExprKey &Other) const {
        return Op == Other.Op && L == Other.L && R == Other.R;
    }
};

struct ExprKeyHash {
    std::size_t operator()(const ExprKey &Key) const {
        std::size_t Hash = std::hash<unsigned>()(Key.Op);
        Hash ^= std::hash<const void *>()(Key.L) + 0x9e3779b97f4a7c15ULL +
                (Hash << 6) + (Hash >> 2);
        Hash ^= std::hash<const void *>()(Key.R) + 0x9e3779b97f4a7c15ULL +
                (Hash << 6) + (Hash >> 2);
        return Hash;
    }
};

// 交换律规范化：把可交换操作的两个操作数按固定顺序排列，
// 使 a+b 与 b+a（以及 a==b 与 b==a）落到同一个键上。
// 注意 SUB、L、G 等不可交换，顺序必须保留。
void CanonicalizeOperands(BinaryInst *Binary, Value *&LHS, Value *&RHS) {
    if (!BinaryInst::isCommutativeOp(Binary->getOp())) return;
    if (std::less<const Value *>()(RHS, LHS)) std::swap(LHS, RHS);
}

} // namespace

PassResult LocalGVNPass::run(Function &F, FunctionAnalysisPassManager &FAM) {
    (void)FAM;

    bool Changed = false;
    // 块内值编号：每个基本块独立维护一张“表达式 -> 代表值”表，
    // 表在一个块处理完后丢弃，因此不做跨块（更不做跨 PHI）的编号。
    for (BasicBlock *BB : F) {
        if (!BB) continue;
        std::unordered_map<ExprKey, Value *, ExprKeyHash> Table;

        // 先快照块内指令：消除过程中会删除指令，不能边遍历边改链表。
        std::vector<Instruction *> Instructions;
        for (Instruction *I : *BB) {
            if (I) Instructions.push_back(I);
        }

        for (Instruction *I : Instructions) {
            auto *Binary = dynamic_cast<BinaryInst *>(I);
            if (!Binary || !IsValueNumberingCandidate(Binary)) continue;

            Value *LHS = nullptr;
            Value *RHS = nullptr;
            if (!Binary->getOperands(LHS, RHS) || !LHS || !RHS) continue;
            CanonicalizeOperands(Binary, LHS, RHS);

            ExprKey Key{static_cast<unsigned>(Binary->getOp()), LHS, RHS};
            auto It = Table.find(Key);
            if (It == Table.end()) {
                // 首次出现的表达式：登记为代表值。
                Table.emplace(Key, Binary);
                continue;
            }
            if (It->second == Binary) continue;

            // 已存在同一表达式的代表值：把本指令的所有使用者改指过去，
            // 再删除本指令。必须先重定向全部使用者再 delete，
            // 因为删除一个仍有使用者的 Value 会连带删除那些使用者。
            Binary->replaceAllUsesWith(It->second);
            delete Binary;
            Changed = true;
        }
    }

    if (!Changed) return PassResult::unchanged();

    // 本 pass 只改数据流,不动 CFG：支配树与循环信息依旧有效，可继续复用。
    PreservationStatus PA;
    PA.keep<DominantAnalysis>();
    PA.keep<LoopAnalysis>();
    return PassResult::changed(std::move(PA));
}
