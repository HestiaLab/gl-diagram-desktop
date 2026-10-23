# 测试

使用 CTest 进行自动化测试。

```sh
ctest --preset ninja-debug --output-on-failure
```

命令详情见 [CMake 指南](cmake.md)。

## 测试框架

测试使用 vendored 的 [Unity](https://github.com/ThrowTheSwitch/Unity) 框架
（MIT，位于 `tests/vendor/unity/`）。每个测试在 `UNITY_BEGIN()` 与
`UNITY_END()` 之间用 `RUN_TEST()` 注册用例，并提供 `setUp()` 与 `tearDown()`。
vendored Unity 源码不参与 `clang-format`、`clang-tidy` 与 `cppcheck`。

## 期望

- 缺陷修复应包含回归测试。
- 新增公共 API 应包含成功与失败用例。
- 测试应断言行为，而非行覆盖率。
- 不启动交互式 OpenGL 窗口；窗口与字体渲染由人工冒烟验证。
- 新增质量门禁必须证明自己能失败：破坏被守护项后应得到指名 `文件:行` 的
  非 0 退出，恢复后必须复绿。

## 自动化测试范围

| 测试 | 覆盖 |
| --- | --- |
| `gldiagramdesktop_version_tests` | 版本查询 API 与生成头一致、版本串形如 semver |
| `gldiagramdesktop_version_consistency` | CMake、生成头、CHANGELOG 与 `--version` 一致 |
| `gldiagramdesktop_decl_def_check` | `include/` 的公共声明在 `src/` 均有定义 |
| `gldiagramdesktop_docs_check` | 相对链接、锚点、代码围栏与 README 索引覆盖 |
| `gldiagramdesktop_gate_behaviors` | 门禁脚本自身能失败（unittest） |
| `gldiagramdesktop_cpack_config` | CPack 发布敏感配置 |

CI 不运行 GUI：OpenGL 上下文、字体烘焙与窗口布局都依赖图形会话，无法在
无头 runner 上稳定复现。

## 手动检查

以下行为无法通过 CTest 验证，PR 中需记录命令、平台与观察结果：

1. 启动应用，确认窗口标题为“GlDiagramDesktop <版本> - 绘图画布”。
2. 确认“显示”面板与“点击我”按钮中的中文正常显示（无方块或缺字）。
3. 点击按钮，确认控制台输出“按钮已点击”。
4. 记录启动日志中的 OpenGL 版本、窗口/帧缓冲尺寸与字体路径。
5. 如改动字体或打包，执行一次安装并运行安装目录中的可执行文件。

截图核验时请使用 DPI-aware 的截图方式；普通截图工具在系统缩放下可能产生
坐标错位。

## 静态分析

CI 运行 clang-format、clang-tidy、cppcheck 与 YAML lint。本地 clang-tidy
可选，因为工具可用性因平台而异：

**Linux/macOS**

```sh
./scripts/check.sh --enable-tidy
```

**Windows PowerShell**

```powershell
./scripts/check.ps1 -EnableTidy
```

`third_party/` 与 `tests/vendor/` 不在扫描面内；不要为了让检查通过而格式化
它们的代码。

## 需要运行的检查

| 变更类型 | 最低限度的有效检查 |
| --- | --- |
| C 源码或公共头文件 | `./scripts/check.ps1` 或 `./scripts/check.sh` |
| 字体、窗口或渲染改动 | 上述检查 + 手动启动与截图核验 |
| 安装或 CPack 改动 | `ninja-release` + `cmake --install` 安装冒烟 |
| CI 或 YAML 改动 | `git diff --check` 与 CI 静态分析 |
| 发布敏感改动 | `ninja-release`、`ninja-asan` 与安装冒烟 |

## 覆盖率

`scripts/coverage_ratchet.py` 是可选入口，默认不接入 CI 与 check 脚本。
覆盖率基线只能有唯一平台口径，更新时请在基线文件中注明取数平台。
