# 编译器(Compiler)

## 序言

欢迎来到焦糖2026编译器环节。

如果你对传统计算机底层/计算机体系结构/底层软件感兴趣，欢迎进入Compiler的世界。

如果你在做完整个题目之后仍觉得自己对这个方向非常感兴趣，可以到[CS自学指南](https://csdiy.wiki/%E4%BD%93%E7%B3%BB%E7%BB%93%E6%9E%84/DDCA/)进行更加深入的学习，或者通过招新题进入工作室获得学长更加细致的指导（墙裂建议）。

**在答题之前，我们先做几点做题规则要求：**

1. 出题者**强烈建议**大一/大二同学使用Agent/AI辅助完成题目，但是，需要对Agent/AI生成的东西**有深入的理解！！！！**

   > 什么是深入的理解：
   >
   > 如果使用AI/Agent辅助编码：请你在面试时指认出哪些是AI生成，哪些是你自己编码，你所做的创新性工作有哪些？
   >
   > 如果使用AI/Agent辅助查询资料/概念理解：请你给出人工概括，或者在面试时能对相关概念给出清晰的解答
   >

2. 注意：**”没有全部完成“和“每道题都做了表面的一点工作”**有着本质的区别，如果没做完，也是可以进入面试，或者有很大机会进入JOTang的，但是如果对概念或者每道题都浅尝辄止，那么可能会被直接淘汰

3. 提交目录**固定为**：

   ```
   Compiler-2026/
   │
   ├── task0
   ├── task1
   ├── task2
   ├── task3
   └── task4
   ```

4. 题目难度不是依次递增的，只是按照编译的一个基本流程去出题，所以遇到不懂的地方可以后做，回头再精进时，可能会有更加深入的理解

**前置：**做本题之前需要对C++以及CMAKE，数据结构有着基本的了解

> 如果你已熟悉，可以跳过前置

C++以及CMAKE资源：

- [C++ 码农论坛](https://www.bilibili.com/video/BV1o8411x7K3/?spm_id_from=333.337.search-card.all.click)
- [CMAKE -- From上交](https://www.bilibili.com/video/BV14h41187FZ/?spm_id_from=333.337.search-card.all.click&vd_source=f5ae372389861ae01366811439fcc342)

数据结构资源：

- [数据结构](https://www.hello-algo.com/chapter_hello_algo/)

在答题之前，请你确保你已经fork/clone了答题仓库：[JOTangCompilerRecruitment](https://github.com/lyjy051219/JOTangCompilerRecruitment)

## TASK0：观看[编译器介绍视频](https://www.bilibili.com/video/BV1D84y1y73v/?spm_id_from=333.788&vd_source=91d1c47a53010e103a5d463866fa18ac)并使用AI/Agent理解编译器的概念

请解答如下问题：以task0.md的文档形式放在task0子目录之下，并在文档最后给出你所参考的相关资料的链接

**注：**不要只给出下面几个问题的答案，出题人更希望看到你们基于这些问题形成的**知识体系**

1. 编译器和解析器的区别？JIT和AOT区别？链接器又是什么？请你在文档之中叙述一下C语言需要经过几个步骤被转换到机器代码的?
2. GCC和LLVM在架构上面的区别？为什么LLVM的架构更加清晰？
3. 解释一下视频中提到的LLVM IR的相关特性，包括但不限于SSA,PHI函数,CFG(Module/Function/BasicBlock/Instruction等),Pass,Value/Use/User,TypeSystem,CFG遍历与侵入式链表等
4. 数据流中User的实现一般等同于CFG控制流中的什么结构？
5. 解释一下前中后端的流程，以及对应流程的主要工作是什么
6. **(拓展)**请你调研一下常见的在LLVM IR上进行优化的Pass有哪些，这些Pass的功能都是什么(在LLVM IR层级简单举例进行叙述)？至少五个
7. **(拓展)**请你自行查询资料（b站视频/知乎博客/YouTuBe/其他网站）了解什么是MLIR？MLIR与传统LLVM IR的区别是什么？
8. **(拓展)**MLIR的核心结构（包括但不限于Dialect,Operation,Region,Block,Value,OpOperand/BlockOperand,OpResult,BlockArgument,Attribute）？
9. **(拓展)**MLIR之中的数据流和LLVM IR中的数据流是一样的吗？如果是一样的话，请你对应二者相关的数据流概念
10. **(拓展)**详细分析MLIR与LLVM IR之间控制流相关概念的区别与相似之处
11. **(拓展)**MLIR采用什么结构代替PHI函数？这种结构相比PHI函数有什么好处？这种结构还能统一LLVM IR之中什么结构？
12. **(拓展)**下述给出的特等奖源码中，是如何基于MLIR思想实现自己的高层IR的？请你自行学习 MLIR Scf Dialect,Func Dialect,Arith Dialect，分析作者第一层IR是否是兼容scf Dialect的，并分析源码之中作者基于第一层高层IR做了哪些优化，这些优化的作用是什么，请你举例说明

LLVM IR辅助资料：

- [Value/User/Use解析博客 -- From 21级焦糖学长](https://zhuanlan.zhihu.com/p/666016704)
- [对于LLVM之类的编译器是如何实现在构造 SSA 形式的 IR 的时候，计算出 def-use 链？](https://www.zhihu.com/question/41999500/answer/93243408)
- [什么是SSA](https://fail.lingfei.xyz/_posts/SSA/)
- [Use-Def链](https://www.cnblogs.com/jourluohua/p/14556253.html)
- [调试LLVM如何生成SSA](https://blog.csdn.net/dashuniuniu/article/details/103389157)
- [2019 EuroLLVM Developers’ Meeting: V. Bridgers & F. Piovezan “LLVM IR Tutorial - Phis, GEPs ...”](https://www.youtube.com/watch?v=m8G_S5LwlTo&t=249s)
- [深入理解LLVM的Value、User、Use — learning-notes v0.1 documentation](https://laity000-learning-notes.readthedocs.io/en/latest/llvm/llvm_value.html)
- [SSA介绍](https://ssa.to/static-analysis-guide/intro)
- [CFG遍历与侵入式链表](https://zhuanlan.zhihu.com/p/664407911)

MLIR一些辅助资料：

- [MLIR核心概念，Pass模式](https://zhuanlan.zhihu.com/p/1974197504993150241)

- [MLIR官方文档](https://mlir.llvm.org/)

- [Hands-On Practical MLIR Tutorial](https://github.com/KEKE046/mlir-tutorial#mlir-%E7%AE%80%E4%BB%8B)

- [一些优秀MLIR资料汇总](https://www.zhihu.com/question/435109274/answer/3585914452)

- [2025年毕昇杯RISCV赛道特等奖编译器源码 -- From黄越](https://github.com/AdUhTkJm/sysy-competition)

## TASK1：前端

在目前的工业级编译器以及毕昇杯等编译器比赛之中，我们经常采用相关工具（而不是采用手搓的形式）实现词法分析->语法分析->生成AST的过程，这里**你需要了解：**

1. 什么是Flex/Bison？二者分别对应前端的什么流程？
2. 什么是正则表达式？终结符？非终结符？
3. 什么是文法？
4. 请你简述Bison与Flex协作生成AST的过程

**请你实现：**

请你阅读[SysY2022](https://gitlab.eduxiji.net/csc1/nscscc/compiler2025/-/blob/main/SysY2022%E8%AF%AD%E8%A8%80%E5%AE%9A%E4%B9%89-V1.pdf?ref_type=heads)以及[SysY Runtime](https://gitlab.eduxiji.net/csc1/nscscc/compiler2025/-/blob/main/SysY2022%E8%BF%90%E8%A1%8C%E6%97%B6%E5%BA%93-V1.pdf?ref_type=heads)，请你在对应task1目录下完成针对SySY2022语言的词法分析->语法分析->AST生成，并调用目录下的脚本进行批量测试，验证AST生成的正确性

> 可以使用给定 AST，也可以自行设计内部 AST。自建 AST 必须提供适配器，输出 [标准 S-expression 协议](Compiler-2026/task1/docs/AST_FORMAT.md) 规定的节点、空值、表达式结构与行号，统一通过原 200 个摘要用例和小型协议用例；内部类设计不影响评分。可调整入口与构建，但不能修改参考答案或以固定输出替代解析。Runtime 中的 `putf` 不在本题验收范围。

**参考资料：**

- [自己动手写编译器](https://pandolia.net/tinyc/index.html)
- [Flex/Bison使用教程](https://www.ljjyy.com/archives/2023/05/100671)
- [Flex](https://github.com/westes/flex)
- [Bison手册](https://www.gnu.org/software/bison/manual/)

## TASK2：中端

相信你在上面两节中以及详细了解了前端的流程以及中端IR的基本架构，这里我们不再展开分析遍历AST生成IR的过程，接下来让我们进入中端优化环节

在出题人的项目之中，IR生成的是非SSA形式的LLVM IR，直到Dominant以及Mem2Reg构建之后，中端才真正进入SSA LLVM IR

请你阅读TypeSystem，BaseManager，CFG，**结合Task0请你分析：**

- TypeSystem是如何实现的？简述TypeSystem使用的设计模式
- 画出Value/Use/User的类关系，CFG关系

请你阅读Dominant，Mem2Reg，请你**解答：**

1. 在出题人的项目之中采用的支配树算法是什么？另外请你了解一下 Lengauer-Tarjan Dominators Algorithm(**可以阅读下面GPT5.0编译器的源码**)，解析二者算法流程并对比优劣
3. 请你再次深入探索PHI函数，分析LLVM IR中PHI的强大作用，探究在编译器数据流优化中发挥的作用
4. 请你查询相关资料，阅读出题人的项目，分析Mem2Reg流程，并详细分析迭代支配边界和Live集合在PHI函数插入中发挥的作用

**Dominant&Mem2Reg参考资料：**

- [Mem2Reg源码解析](https://llvm-study-notes.readthedocs.io/en/latest/ssa/Mem2Reg.html)
- [支配树](https://www.zhihu.com/project-square)
- **(答题必备github源码)**[GPT5.0源码 -- From焦糖22级学长](https://github.com/lyh552506/miniC-compiler)

**NewPassManager：**

在上面的学习中，你已经学习过，中端本质是由若干个Pass组成的，而目前你肯定会疑惑，中端组织若干个Pass，并确定相关Pass顺序的数据结构是什么样子的呢

请你结合下面给出的参考资料以及出题人项目的源码**解答：**

- 出题人的项目是如何构建中端Pass Pipeline的？
- CRTP设计模式，这种设计模式的好处是什么
- 类型擦除
- 出题人项目源码之中PassID这个空类的作用是什么？
- AnalysisPass注册的作用？项目中是如何实现注册的？
- 分别分析TransformPass/AnalysisPass调用的整个流程
- **(拓展)**如果你感兴趣，请阅读LLVM相关NewPassManager源码，探寻自动失效机制

**参考资料:**

[NewPassManager讲解](https://zhuanlan.zhihu.com/p/696028293)

相信你到现在已经对编译器前端以及中端SSA形式的LLVM IR已经很熟悉了，接下来让我们进入编译器最核心的环节：**中端优化**

- **请你按照**task2目录下的README中的要求实现相关要求

- 阅读下面的博客，解答：
  1. 什么是仿射变换？
  2. 多面体循环优化是解决什么类型的循环并行问题的？请你了解一下循环展开，分析多面体优化与循环展开所针对的循环类型的不同之处
  3. **(拓展)**请你自行搜寻资料，阅读多面体编译优化相关源码，能否举例一个典型的多面体求解器的源码实现，并分析它是如何在编译器中起作用的？

**多面体初探参考资料：**

- [浅析多面体编译优化](https://zhuanlan.zhihu.com/p/1959822569080349209) 
- [编译器领域的多面体模型](https://zhuanlan.zhihu.com/p/310142893)

## TASK3：后端

如果你做到这一部分，相信你已经对编译器前端中端有着更加深入的了解了，在一次编译之中，传给后端的是中端的Module数据结构，这个结构本质就是优化过的内存形式的LLVM IR，那么编译器后端是如何生成对应硬件平台的汇编代码的呢，下面我们以RISCV汇编为例，进入编译器后端！

**基础运行实验与配置环境：**

按 [Task3 实验说明](Compiler-2026/task3/TASK3.md) 完成给出的 C/汇编混合程序交叉编译与 QEMU Linux 用户态执行。统一目标为 RV64GC、LP64D、静态链接；无需启动完整虚拟机，也无需完成自己的后端。

在 `Compiler-2026/task3` 下运行 `bash check_environment.sh`，三组输入 `17 20`、`-5 8`、`0 0` 应分别输出 `42`、`8`、`5`，退出码均为 0。报告需解释参数/返回值、栈帧、`ra` 与 `s0` 保存恢复，并先手算再验证一组独立输入。只提供 PASS 输出不算完成。

- [QEMU 用户态模拟说明](https://www.qemu.org/docs/master/user/main.html)
- [RISC-V psABI 调用约定](https://riscv-non-isa.github.io/riscv-elf-psabi-doc/)
- [GNU RISC-V 工具链构建说明](https://github.com/riscv-collab/riscv-gnu-toolchain)

**熟悉RISCV指令集&RISCV硬件特性&MIR：**

请你观看[RISCV指令集](https://www.bilibili.com/video/BV1Q5411w7z5/?p=12&vd_source=91d1c47a53010e103a5d463866fa18ac)相关视频，或者自行查阅[RV BOOK](https://www.cs.sfu.ca/~ashriram/Courses/CS295/assets/books/rvbook.pdf)/[RV BOOK中文版](http://riscvbook.com/chinese/RISC-V-Reader-Chinese-v2p1.pdf)学习相关RISCV指令集,请你在Markdown之中叙述

- 物理寄存器：Caller-Saved与Callee-Saved有什么区别，各大物理寄存器的功能都是什么

- 学习：栈帧、参数/返回、全局地址、指令、MIR

**寄存器分配：**

在SSA LLVM IR之中，我们默认所有变量都是存储在虚拟寄存器之上的，那么自然知道中端默认寄存器是无限的，但是在真实硬件环境之中，寄存器的数量往往是有限的，这时就出现一个问题，如何在程序运行的过程之中，将程序亟待使用的变量，高效放在寄存器上执行，从而最大化减少访存时延

**所以请你：**

- 解释虚拟/物理寄存器、活跃区间和 spill；从线性扫描、图着色中任选一种，用小例子说明如何处理有限寄存器。
- 阅读下述论文、博客与源码，对比线性扫描、图着色、LLVM Greedy 的思想、spill 选择和编译开销，并加入自己的思考。不要求实现三种分配器。

**参考资料：**

- [线性扫描](https://dl.acm.org/doi/pdf/10.1145/277650.277714)
- [图着色](https://dl.acm.org/doi/pdf/10.1145/229542.229546)
- [Greedy Register Allocation in LLVM 3.0](https://blog.llvm.org/2011/09/greedy-register-allocation-in-llvm-30.html?utm_source=chatgpt.com)
- [[llvm-dev] LLVM's Greedy Register allocator paper details](https://lists.llvm.org/pipermail/llvm-dev/2022-January/154647.html?utm_source=chatgpt.com)
- [LLVM Greedy寄存器分配算法实现](https://github.com/llvm/llvm-project/blob/main/llvm/lib/CodeGen/RegAllocGreedy.cpp?utm_source=chatgpt.com)


## TASK4：合规优化

相信做到本节，你已经对编译器与优化有着比较清晰且深入的认识了

所以请你阅读[关于编译优化合理性与违规行为认定的说明](https://gitlab.eduxiji.net/csc1/nscscc/compiler2026/-/blob/main/%E5%85%B3%E4%BA%8E%E7%BC%96%E8%AF%91%E4%BC%98%E5%8C%96%E5%90%88%E7%90%86%E6%80%A7%E5%8F%8A%E7%9B%B8%E5%85%B3%E8%BF%9D%E8%A7%84%E8%A1%8C%E4%B8%BA%E8%AE%A4%E5%AE%9A%E7%9A%84%E8%AF%B4%E6%98%8E.pdf)，[模式匹配违规行为大赏1](https://www.zhihu.com/question/664985544/answer/3608126911)，[模式匹配违规行为大赏2](https://www.zhihu.com/question/664985544/answer/3604997741)，简要谈谈什么样的优化是合规性优化，什么样的优化是违规的，投机的违规优化

**请你：**

- 将你对上述优化行为的思考写入你的Markdown
- 再次**人工审查**你与Agent编写的优化代码，规避违规优化行为的出现

## 提交内容

请将已完成任务的报告和程序源码提交到 GitHub **Public** 仓库，将仓库链接与验收 commit 提交到招新网站。

允许 AI 辅助，但须能解释生成内容与人工验证过程。不得用预期结果冒充实测结果，不提交二进制、build 目录和临时调试产物。

如果对招新题有任何疑问/对Compiler感兴趣，可以随时联系出题人：3117560943(QQ)

















