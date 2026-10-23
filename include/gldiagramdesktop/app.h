#ifndef GLDIAGRAMDESKTOP_APP_H
#define GLDIAGRAMDESKTOP_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* 启动应用主循环，返回进程退出码。 */
int gldiagramdesktop_run(void);

/* 返回编译期写入的版本字符串（形如 "0.1.0"）。 */
const char *gldiagramdesktop_version(void);

#ifdef __cplusplus
}
#endif

#endif /* GLDIAGRAMDESKTOP_APP_H */
