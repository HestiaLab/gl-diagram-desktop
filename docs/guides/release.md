# 发布流程

本项目不强制使用特定发布工具。一次发布应满足以下步骤。

## 发布前

1. 确认 `CMakeLists.txt` 中的 `project(... VERSION ...)`、生成头文件、
   `CHANGELOG.md` 最新段落与 `gldiagramdesktop --version` 四处一致。
2. 更新 CHANGELOG：新增段落用中文书写，已发布段落保留发布时的原文。
3. 行为、依赖、安装内容或公共 API 变化时同步更新文档。
4. 从干净的 `main` 分支开始，确认 CI 全部通过。

## 验证

**Linux/macOS**

```sh
./scripts/check.sh --enable-tidy
cmake --preset ninja-release
cmake --build --preset ninja-release
ctest --preset ninja-release --output-on-failure
cmake --install build/ninja-release --prefix install-smoke
./install-smoke/bin/gldiagramdesktop --version
```

**Windows PowerShell**

```powershell
./scripts/check.ps1 -EnableTidy
cmake --preset ninja-release
cmake --build --preset ninja-release
ctest --preset ninja-release --output-on-failure
cmake --install build/ninja-release --prefix install-smoke
./install-smoke/bin/gldiagramdesktop.exe --version
```

至少在一个带图形会话的平台上完成手动 GUI 冒烟：

- 窗口标题显示版本号；
- “显示”面板与“点击我”按钮中文正常；
- 点击按钮后控制台输出“按钮已点击”。

字体或打包相关改动的发布还应确认安装目录包含 `bin/fonts/` 与字体许可证。

## 打包

CPack 由 `cmake/Packaging.cmake` 配置：

- 所有平台默认生成 TGZ；
- Linux 上检测到 `dpkg`/`rpmbuild` 时额外生成 DEB/RPM；
- 源码包为 TGZ/ZIP。

```sh
cd build/ninja-release
cpack
```

## 打标签

```sh
git tag -a v0.1.0 -m "v0.1.0"
git push origin v0.1.0
```

标签必须打在干净、通过 CI 的 `main` 提交上。未经维护者同意，不要发布由 AI
辅助完成的改动。
