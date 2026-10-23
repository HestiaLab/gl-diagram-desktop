# 0002 应用型项目适配

## 状态

已接受。

**日期**：2026-10-23

## 背景

`c-project-standard` 的生成骨架围绕「核心库 + 可选示例可执行 + CMake 包导出」
设计。GlDiagramDesktop 是纯桌面应用：没有稳定的公共库 API，也不需要被其他
CMake 项目 `find_package` 消费。

同时，项目依赖 GLAD、KHR 与 Nuklear 三类 vendored 代码，并以中文作为唯一
界面与文档语言。这些约束都会与默认模板结构产生摩擦。

## 决策

1. **纯应用形态**：只构建 `gldiagramdesktop` 可执行文件，不建导出库、不安装
   公共头、不生成 `GlDiagramDesktopConfig.cmake`。安装内容为可执行文件、
   界面字体与文档。
2. **最小公共 API**：`include/gldiagramdesktop/app.h` 只暴露
   `gldiagramdesktop_run()` 与 `gldiagramdesktop_version()`，使 `decl_def`
   门禁仍能守护「声明必须有实现」，同时不虚构库接口。
3. **vendored 代码隔离**：GLAD、KHR、Nuklear 及其实现胶水位于 `third_party/`，
   由独立 OBJECT 目标编译。质量门禁的扫描面仍是 `include/ src/ tests/`，
   因此 `third_party/` 天然不在 clang-format、clang-tidy、cppcheck 范围内，
   无需修改模板脚本的扫描逻辑。
4. **质量门禁全量保留**：`decl_def`、版本一致性、文档、门禁自测、CPack 配置
   五类门禁全部启用。门禁自测中「定位公共函数实现」的逻辑由硬编码
   `src/**/example.c` 改为按 `include/` 声明匹配，以适应应用型目录。
5. **中文唯一**：文档、界面、命令行提示均为中文；不再维护双语镜像。模板自带
   脚本与 CI 的内部英文注释保持原样，避免与上游模板漂移。
6. **字体随包分发**：界面字体使用 Noto Sans SC Regular（OFL），运行时按可执行
   文件相对路径查找，并用 CJK 基本区图集烘焙，避免依赖系统字体差异。

## 后果

- 下游无法通过 `find_package` 消费本项目；若未来需要库化，应新增 ADR 并重新
  引入导出目标与包冒烟测试。
- CI 的 package-smoke 作业改为 `cmake --install` 安装冒烟，subproject 冒烟不再
  适用。
- 仓库增加约 11MB 字体文件；启动时烘焙 CJK 图集带来约 0.5 秒（release）额外
  开销与数十 MB 显存占用。
- `third_party/` 的依赖升级需要单独提交，并在 `third_party/README.md` 记录版本
  与许可证。
