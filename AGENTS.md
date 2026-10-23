# AGENTS.md

面向在 GlDiagramDesktop 项目中工作的 AI 代理。

## 必读

修改文件之前，先阅读：

- `CONTRIBUTING.md`
- `docs/guides/testing.md`
- `docs/guides/cmake.md`

涉及架构或结构变化的改动，还需阅读：

- `docs/adr/README.md`
- `docs/adr/0002-application-shape.md`

涉及依赖、打包或安全的工作，还需阅读：

- `third_party/README.md`
- `SECURITY.md`
- `docs/guides/release.md`

## 项目形状

C11 + CMake + OpenGL 3.3 + Nuklear 的纯桌面应用。

```text
include/gldiagramdesktop/   公共 API（app.h）
src/                        应用入口与私有实现
src/gldiagramdesktop/       应用逻辑、平台适配、版本查询
third_party/                vendored GLAD/KHR/Nuklear（不参与项目警告）
tests/                      CTest 测试（Unity）
cmake/                      构建模块
scripts/                    本地引导、检查与质量门禁
docs/                       中文文档
res/                        Windows 版本资源与界面字体
```

## 构建与验证

```sh
cmake --preset ninja-debug
cmake --build --preset ninja-debug
ctest --preset ninja-debug --output-on-failure
```

Windows PowerShell 使用 `./scripts/check.ps1`，Linux/macOS 使用
`./scripts/check.sh`。提交前运行带 clang-tidy 的完整检查。

涉及窗口、字体或渲染的改动，必须启动应用并截图核验中文显示；CTest 不覆盖
GUI。

## 规则

- 公共 API 属于 `include/gldiagramdesktop/`，私有实现与私有头属于 `src/`。
- `third_party/` 与 `tests/vendor/` 不参与格式化与静态分析，不得因无关改动
  重写它们。
- 文档、界面与命令行输出只使用中文；不要重新引入英文镜像。
- 字体变更需同步更新 `res/fonts/README.md` 的来源与 SHA-256。
- 测试应断言行为，而不是偶然的实现细节。
- 不为了让改动通过而放宽警告、测试、静态分析或 CI。
- 修改构建、测试、安装或安全策略时同步更新文档。

许可证：MIT；界面字体为 SIL OFL 1.1。
