# 界面字体

本目录随程序分发界面使用的中文字体，满足“项目仅提供中文界面”的要求。

## 文件

| 文件 | 说明 |
| --- | --- |
| `NotoSansSC-Regular.ttf` | Noto Sans SC Regular 静态实例，SIL Open Font License 1.1 |
| `OFL.txt` | 字体许可证原文 |

## 来源与再生成

原始字库来自本机安装的 `NotoSansSC-VF.ttf`（Noto Sans SC 可变字体，glyf 轮廓，
31036 个字形）。静态实例由 fonttools 生成：

```sh
python -m fontTools.varLib.instancer NotoSansSC-VF.ttf wght=400 -o NotoSansSC-Regular.ttf
```

生成结果 SHA-256：

```text
3e0954de2a02a487803a65716bb5244a36f5d83797432d84bf7cccc9962d948f
```

界面图集通过 `nk_font_chinese_glyph_ranges()` 烘焙 CJK 基本区，覆盖日常中文与
全角标点。修改字体文件时请同步更新本页的记录。

## 分发方式

构建时复制到可执行文件同级的 `fonts/` 目录，安装时随程序一起分发；运行时可用
`GLDIAGRAMDESKTOP_FONT` 环境变量覆盖字体路径。
