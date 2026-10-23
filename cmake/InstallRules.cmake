# 应用型项目的安装规则。
#
# 本项目只分发可执行文件、界面字体与文档，不导出库或 CMake 包。

include(GNUInstallDirs)

if(TARGET gldiagramdesktop)
    install(TARGETS gldiagramdesktop
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        BUNDLE DESTINATION .
    )
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/res/fonts")
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/res/fonts/"
        DESTINATION "${CMAKE_INSTALL_BINDIR}/fonts"
    )
endif()

install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"
    "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
)
