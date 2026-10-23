#ifndef GLDIAGRAMDESKTOP_PLATFORM_H
#define GLDIAGRAMDESKTOP_PLATFORM_H

#include <stddef.h>

#define GLDIAGRAMDESKTOP_PATH_MAX 4096

/*
 * 解析界面字体路径：优先使用 GLDIAGRAMDESKTOP_FONT 环境变量，
 * 否则查找可执行文件同级的 fonts/NotoSansSC-Regular.ttf。
 * 成功返回 0，失败返回 -1。
 */
int gldiagramdesktop_font_path(char *buffer, size_t size);

#endif /* GLDIAGRAMDESKTOP_PLATFORM_H */
