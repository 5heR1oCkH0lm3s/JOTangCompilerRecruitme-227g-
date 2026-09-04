# 前端测试说明

`testcases26` 中原有的 `.out` 是程序执行结果，无法证明前端生成了正确 AST。本任务采用三层检查：

1. 前端对每个 `.sy` 返回成功，并输出单行 `(CompUnit ...)`；
2. 对规范化 AST 的完整 stdout 计算 SHA-256，与 `ast.sha256` 中的参考摘要比较。
3. 对 `tests/negative` 中的非法程序检查非零退出码。

摘要比较可以发现声明顺序、表达式优先级、数组维度、初始化列表、分支结构和函数调用等 AST 错误，同时避免在仓库中保存体积较大的完整 `.ast` 文件。

`golden/` 中额外提供了几个可直接阅读的完整 AST 示例。对应示例发生差异时，脚本会打印 unified diff；其余大型用例只显示摘要和实际 AST 前缀。

运行全部测试：

```bash
python3 test_frontend.py
```

测试脚本只负责运行测试，不会调用 Flex、Bison 或 CMake。运行前必须按照主 README 手工生成代码并完成构建，默认读取 `build/sysy_frontend`。

常用选项：

```bash
# 只跑 10 个用例
python3 test_frontend.py --max-cases 10

# 只跑路径包含 op_priority 的用例
python3 test_frontend.py --filter op_priority

# 使用其他位置的前端可执行文件
python3 test_frontend.py --compiler path/to/sysy_frontend

# 只检查能否生成基本 AST，不比较参考摘要
python3 test_frontend.py --no-golden

# 暂时跳过非法程序拒绝测试
python3 test_frontend.py --no-negative
```

测试脚本不读取 `.in`，也不比较 `.out`，因为本任务不会运行程序。
