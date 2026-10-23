# 跨平台安装与 CPack 打包入口。
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/InstallRules.cmake")

set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_VENDOR "${GLDIAGRAMDESKTOP_PROJECT_AUTHOR}")

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # 当前 CMake 的 productbuild 生成器可能产出空分发包，本地只出 TGZ。
    set(CPACK_GENERATOR "TGZ")
else()
    find_program(CPACK_DEB_EXECUTABLE dpkg)
    find_program(CPACK_RPM_EXECUTABLE rpmbuild)
    set(CPACK_GENERATOR "TGZ")
    if(CPACK_DEB_EXECUTABLE)
        list(APPEND CPACK_GENERATOR "DEB")
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${GLDIAGRAMDESKTOP_PROJECT_AUTHOR}")
        set(CPACK_DEBIAN_PACKAGE_SECTION "graphics")
        set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    endif()
    if(CPACK_RPM_EXECUTABLE)
        list(APPEND CPACK_GENERATOR "RPM")
        set(CPACK_RPM_PACKAGE_LICENSE "${GLDIAGRAMDESKTOP_PROJECT_LICENSE}")
        set(CPACK_RPM_PACKAGE_GROUP "Applications/Graphics")
    endif()
endif()

set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_SOURCE_IGNORE_FILES
    "/build/"
    "/_CPack_Packages/"
    "/\\\\.git/"
    "/\\\\.vs/"
    "/\\\\.cache/"
    "/install/"
    "/install-"
    "/dist/"
    "/out/"
    "\\\\.swp$"
    "\\\\.swo$"
    "\\\\.user$"
    "\\\\.idx$"
)

include(CPack)
