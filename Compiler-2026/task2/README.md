### README

1. 请你先阅读task2目录下的项目，分析其CMAKE结构，然后自行构建其整个项目，并请你自行配置`launch.json`和`tasks.json`文件，从而在vscode可以调试整个项目

2. 请你阅读LoopInfo，LoopSimplify，LCSSA等循环规范化pass，分析什么是循环规范形态，画出这几个pass在控制流和数据流优化前后的对比图

3. 请你查阅相关资料(LLVM源码/知乎博客等)，借鉴相关思路，实现GVN(块内公共子表达式消除)以及LICM(循环不变量外提)，并使用task2目录下的脚本进行测试，测试样例放在testcases26目录中，你交上来的pass必须是功能正确的，如果可以的话，再试试优化一下性能？

   > 测试脚本所用的runtime.ll注入包含 x86-64 属性，经检验在linux/Win环境下可用，如果你是macOS/ARM主机，如果出现因为runtime.ll导致的测试问题，请你自行查阅资料进行修改

4. **(拓展，极难)**如果你对高层IR感兴趣，可以基于task2目录下的编译器前中端构建属于你的高层IR，并实现SCF/CF层级优化，比如将while循环转成for循环，在for循环层级上实现LoopFuse优化

   > 在这一步，你至少需要实现一套类似MLIR的数据结构来充当你的高层IR，并且需要阅读IR生成部分代码并做修改，首先将AST Lower到MLIR，再Lower到LLVM IR，然后在MLIR上搭建属于你的PassBuilder管理Pass，然后进行实现LoopFuse

