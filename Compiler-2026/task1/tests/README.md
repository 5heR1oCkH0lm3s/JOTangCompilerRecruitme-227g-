# 前端测试说明

所采用的测试样例存放在`testcases26`目录之下

本任务采用三层检查：

1. 前端对每个 `.sy` 返回成功，并输出单行 `(CompUnit ...)`；
2. 对规范化 AST 的完整 stdout 计算 SHA-256，与 `ast.sha256` 中的参考摘要比较。

`golden/` 中额外提供了几个可直接阅读的完整 AST 示例。对应示例发生差异时，脚本会打印 unified diff；其余大型用例只显示摘要和实际 AST 前缀。

运行全部测试：

```bash
python3 test_frontend.py
```

测试脚本只负责运行测试，不会调用 Flex、Bison 或 CMake。运行前必须按照主 README 手工生成代码并完成构建，默认读取 `build/sysy_frontend`

