# AGENTS.md

面向在 GlDiagramDesktop 项目中工作的 AI 代理。

## 必读

- `CONTRIBUTING.md`
- `docs/guides/testing.md`
- `docs/guides/cmake.md`
- `SECURITY.md`

## 项目形状

```text
include/        公共头文件
src/            实现
tests/          CTest 测试
cmake/          构建模块
scripts/        本地引导与检查脚本
docs/           项目文档
```

## 验证

```sh
cmake --preset ninja-debug
cmake --build --preset ninja-debug
ctest --preset ninja-debug --output-on-failure
```

## 规则

- 公共 API 属于 `include/`，私有实现属于 `src/`。
- 测试应断言行为，而不是偶然的实现细节。
- 不为了让改动通过而放宽警告、测试或 CI。
- 修改构建、测试、发布或安全策略时同步更新文档。

许可证：MIT
