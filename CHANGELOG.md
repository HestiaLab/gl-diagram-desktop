# 更新日志

> 新增段落用中文书写；已发布段落保留发布时的原文，不重写。

## 0.1.0 — 2026-10-23

- 项目更名为 GlDiagramDesktop（仓库 `gl-diagram-desktop`），版本重新起算。
- 迁移到 c-project-standard v0.2.1 工程结构：模块化 CMake、Ninja 预设、
  CTest、质量门禁脚本与三平台 CI。
- 采用纯应用形态：仅构建并安装 `gldiagramdesktop`，不导出 CMake 包。
- GLAD、KHR 与 Nuklear 迁入 `third_party/`，由独立 OBJECT 目标编译。
- 新增公共 API `gldiagramdesktop_run()` 与 `gldiagramdesktop_version()`，
  并提供 `--help` / `--version` 命令行参数。
- 界面、窗口标题、错误信息与文档改为中文，不再维护英文版本。
- 内置 Noto Sans SC Regular 字体，按 CJK 基本区烘焙图集，支持
  `GLDIAGRAMDESKTOP_FONT` 覆盖。
- 安装规则分发可执行文件、界面字体与文档，CPack 生成 TGZ（Linux 可选
  DEB/RPM）。
- 新增 ADR 0002，记录应用型适配与 vendored 代码、字体分发决策。
