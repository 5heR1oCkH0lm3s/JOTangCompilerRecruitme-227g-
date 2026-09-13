# 前端测试说明

所采用的测试样例存放在`testcases26`目录之下

本任务采用两层检查：

1. 前端对每个 `.sy` 返回成功，并输出单行 `(CompUnit ...)`；
2. 对规范化 AST 的完整 stdout 计算 SHA-256，与 `ast.sha256` 中的参考摘要比较。

`golden/` 中额外提供了几个可直接阅读的完整 AST 示例。对应示例发生差异时，脚本会显示首个不同字节及邻近内容；其余大型用例显示摘要和实际 AST 前缀。使用下述产物选项可保存完整差异。

运行全部测试：

```bash
python3 test_frontend.py
```

测试脚本只负责运行测试，不会调用 Flex、Bison 或 CMake。运行前必须按照主 README 手工生成代码并完成构建，默认读取 `build/sysy_frontend`


## 小型协议用例

`examples/` 内每个 `.sy` 都配有完整 `.ast`，使用独立 `ast.sha256`，不改变原 200 个摘要。执行 `python3 test_frontend.py --examples`，并继续执行原 200 个用例；两组均属于 Task1 验收。

格式规则见 [AST 输出协议](../docs/AST_FORMAT.md)。`--output-dir build/ast-debug` 可保留完整实际输出与差异；有 golden 的 WA 会显示首个不同字节及邻近文本。指定自定义用例目录时，可用 `--test-root`、`--manifest`、`--golden-root` 明确三者位置。
