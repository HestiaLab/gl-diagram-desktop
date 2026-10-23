# GlDiagramDesktop

基于 OpenGL 3.3 与 Nuklear 即时模式 GUI 的绘图桌面应用。仓库名与远程地址为
`gl-diagram-desktop`，构建产物与公共头目录使用 `gldiagramdesktop` 命名空间。

本项目只提供中文文档与中文界面，不再维护英文版本。

## 快速开始

首次检出先运行 bootstrap，它会检查工具链并完成一次 configure：

**Windows PowerShell**

```powershell
./scripts/bootstrap.ps1
```

**Linux/macOS**

```sh
./scripts/bootstrap.sh
```

日常检查与构建：

```powershell
./scripts/check.ps1
```

```sh
./scripts/check.sh
```

手动执行 CMake 命令：

```sh
cmake --preset ninja-debug
cmake --build --preset ninja-debug
ctest --preset ninja-debug --output-on-failure
```

运行应用：

```sh
./build/ninja-debug/bin/gldiagramdesktop
```

命令行参数：

```text
-h, --help     显示帮助
-v, --version  显示版本号
```

## 环境要求

- CMake 3.21 或更高版本。
- Ninja（默认 preset）与 C11 编译器（MSVC、GCC、Clang、AppleClang）。
- 支持 OpenGL 3.3 的显卡驱动。
- 首次配置需要网络：GLFW 3.3.9 通过 FetchContent 获取；离线环境可用
  `FETCHCONTENT_SOURCE_DIR_GLFW` 指向本地 GLFW 源码。

Linux 还需要 OpenGL 与 X11 开发包，macOS 需要 Xcode Command Line Tools。
详见 [环境配置](docs/guides/environment.md)。

## 项目结构

```text
include/gldiagramdesktop/   公共 API（应用入口与版本查询）
src/                        应用实现
src/gldiagramdesktop/       私有实现与平台适配
third_party/                vendored GLAD、KHR、Nuklear（不参与项目警告）
tests/                      CTest 测试（Unity）
cmake/                      CMake 模块
scripts/                    本地引导、检查与质量门禁脚本
docs/                       中文项目文档
res/                        Windows 版本资源与界面字体
```

## 界面字体

界面使用随包分发的 Noto Sans SC Regular（SIL OFL 1.1），启动时按 CJK 基本区
烘焙图集。字体位于可执行文件同级的 `fonts/` 目录；可用
`GLDIAGRAMDESKTOP_FONT` 环境变量指定其他字体文件。字体来源与再生成方式见
[res/fonts/README.md](res/fonts/README.md)。

## 安装

```sh
cmake --preset ninja-release
cmake --build --preset ninja-release
cmake --install build/ninja-release --prefix install
```

安装内容包括可执行文件、`bin/fonts/` 下的界面字体与文档。本项目是纯应用，
不导出库或 CMake 包。

## 文档

- [CMake 与构建](docs/guides/cmake.md)
- [测试与质量门禁](docs/guides/testing.md)
- [编码规范](docs/guides/coding-style.md)
- [环境配置](docs/guides/environment.md)
- [发布流程](docs/guides/release.md)
- [架构决策记录](docs/adr/README.md)
- [工具链策略 ADR](docs/adr/0001-c11-cmake-toolchain-policy.md)
- [应用型适配 ADR](docs/adr/0002-application-shape.md)
- [工程标准](C_PROJECT_STANDARD.md)
- [贡献指南](CONTRIBUTING.md)
- [安全策略](SECURITY.md)

## 许可证

MIT，见 [LICENSE](LICENSE)。界面字体另受 SIL Open Font License 1.1 约束。
