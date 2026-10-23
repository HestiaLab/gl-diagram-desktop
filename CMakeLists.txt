cmake_minimum_required(VERSION 3.21...3.31)

project(GlDiagramDesktop
    VERSION 0.1.0
    DESCRIPTION "基于 OpenGL 3.3 与 Nuklear 即时模式 GUI 的绘图桌面应用"
    LANGUAGES C
)
set(GLDIAGRAMDESKTOP_PROJECT_AUTHOR "GlDiagramDesktop contributors")
set(GLDIAGRAMDESKTOP_PROJECT_LICENSE "MIT")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(GLDIAGRAMDESKTOP_IS_TOP_LEVEL_DEFAULT ON)
else()
    set(GLDIAGRAMDESKTOP_IS_TOP_LEVEL_DEFAULT OFF)
endif()

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "构建类型" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(GLDIAGRAMDESKTOP_ENABLE_ASAN "启用 AddressSanitizer（支持的平台上）" OFF)
option(GLDIAGRAMDESKTOP_ENABLE_UBSAN "启用 UndefinedBehaviorSanitizer（支持的平台上）" OFF)
option(GLDIAGRAMDESKTOP_ENABLE_COVERAGE "启用覆盖率编译选项（支持的平台上）" OFF)
option(GLDIAGRAMDESKTOP_BUILD_APP "构建 GlDiagramDesktop 应用" ${GLDIAGRAMDESKTOP_IS_TOP_LEVEL_DEFAULT})
option(GLDIAGRAMDESKTOP_BUILD_TESTING "构建 CTest 测试" ${GLDIAGRAMDESKTOP_IS_TOP_LEVEL_DEFAULT})
option(GLDIAGRAMDESKTOP_INSTALL "生成安装规则" ${GLDIAGRAMDESKTOP_IS_TOP_LEVEL_DEFAULT})

set(GLDIAGRAMDESKTOP_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated/include")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/gldiagramdesktop_version.h.in"
    "${GLDIAGRAMDESKTOP_GENERATED_INCLUDE_DIR}/gldiagramdesktop/version.h"
    @ONLY
)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/CompilerOptions.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Dependencies.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Sources.cmake")

set(GLDIAGRAMDESKTOP_THIRD_PARTY_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/KHR"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/nuklear/include"
)

if(GLDIAGRAMDESKTOP_BUILD_APP)
    # vendored 代码单独编译：不施加项目警告与静态分析。
    add_library(gldiagramdesktop_vendor OBJECT ${GLDIAGRAMDESKTOP_VENDOR_SOURCES})
    target_compile_features(gldiagramdesktop_vendor PRIVATE c_std_11)
    target_compile_definitions(gldiagramdesktop_vendor PRIVATE GLFW_INCLUDE_NONE)
    target_include_directories(gldiagramdesktop_vendor
        SYSTEM PRIVATE
            ${GLDIAGRAMDESKTOP_THIRD_PARTY_INCLUDE_DIRS}
            "${glfw_SOURCE_DIR}/include"
    )

    # Windows 版本信息资源
    if(WIN32)
        set(GLDIAGRAMDESKTOP_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
        set(GLDIAGRAMDESKTOP_VERSION_MINOR ${PROJECT_VERSION_MINOR})
        set(GLDIAGRAMDESKTOP_VERSION_PATCH ${PROJECT_VERSION_PATCH})
        configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/res/gldiagramdesktop.rc.in"
            "${CMAKE_CURRENT_BINARY_DIR}/generated/gldiagramdesktop.rc"
            @ONLY
        )
    endif()

    add_executable(gldiagramdesktop
        ${GLDIAGRAMDESKTOP_APP_SOURCES}
        $<TARGET_OBJECTS:gldiagramdesktop_vendor>
    )

    if(WIN32)
        target_sources(gldiagramdesktop PRIVATE
            "${CMAKE_CURRENT_BINARY_DIR}/generated/gldiagramdesktop.rc"
        )
    endif()

    target_compile_definitions(gldiagramdesktop PRIVATE GLFW_INCLUDE_NONE)
    target_include_directories(gldiagramdesktop
        PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/include"
            "${GLDIAGRAMDESKTOP_GENERATED_INCLUDE_DIR}"
    )
    target_include_directories(gldiagramdesktop
        SYSTEM PRIVATE
            ${GLDIAGRAMDESKTOP_THIRD_PARTY_INCLUDE_DIRS}
            "${glfw_SOURCE_DIR}/include"
    )
    target_link_libraries(gldiagramdesktop
        PRIVATE
            glfw
            OpenGL::GL
    )
    if(WIN32)
        target_link_libraries(gldiagramdesktop PRIVATE gdi32)
    endif()
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        target_link_libraries(gldiagramdesktop PRIVATE dl pthread)
    endif()

    gldiagramdesktop_target_defaults(gldiagramdesktop)
    set_target_properties(gldiagramdesktop PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/res/fonts")
        add_custom_command(TARGET gldiagramdesktop POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/res/fonts"
                "$<TARGET_FILE_DIR:gldiagramdesktop>/fonts"
            COMMENT "复制界面字体到输出目录"
        )
    endif()
endif()

if(GLDIAGRAMDESKTOP_BUILD_TESTING)
    enable_testing()
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Tests.cmake")
endif()

if(GLDIAGRAMDESKTOP_INSTALL)
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Packaging.cmake")
endif()

message(STATUS "----------------------------------")
message(STATUS "项目: ${PROJECT_NAME}")
message(STATUS "版本: ${PROJECT_VERSION}")
message(STATUS "系统: ${CMAKE_SYSTEM_NAME}")
message(STATUS "编译器: ${CMAKE_C_COMPILER}")
message(STATUS "----------------------------------")
