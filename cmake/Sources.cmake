# 应用与 vendored 源码清单。
#
# third_party/ 下的代码由独立的 OBJECT 目标编译，不参与项目警告与静态分析。

set(GLDIAGRAMDESKTOP_APP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gldiagramdesktop/app.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gldiagramdesktop/version.c
)

set(GLDIAGRAMDESKTOP_VENDOR_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/src/glad.c
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/nuklear/nuklear_impl.c
)
