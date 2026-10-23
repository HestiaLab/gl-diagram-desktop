# 测试注册辅助函数。
#
# 每个测试源文件生成一个独立可执行文件并注册到 CTest。
# 测试断言使用 Unity（vendored 到 tests/vendor/unity/，MIT）。

set(GLDIAGRAMDESKTOP_UNITY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/vendor/unity/unity.c
)

function(gldiagramdesktop_add_test target)
    cmake_parse_arguments(GLDIAGRAMDESKTOP_TEST
        ""
        ""
        "SOURCES;LIBS"
        ${ARGN}
    )

    if(NOT GLDIAGRAMDESKTOP_TEST_SOURCES)
        message(FATAL_ERROR "gldiagramdesktop_add_test(${target}) requires SOURCES")
    endif()

    add_executable(${target}
        ${GLDIAGRAMDESKTOP_TEST_SOURCES}
        ${GLDIAGRAMDESKTOP_UNITY_SOURCES}
    )

    target_include_directories(${target}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${GLDIAGRAMDESKTOP_GENERATED_INCLUDE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/vendor/unity
    )

    if(GLDIAGRAMDESKTOP_TEST_LIBS)
        target_link_libraries(${target} PRIVATE ${GLDIAGRAMDESKTOP_TEST_LIBS})
    endif()

    gldiagramdesktop_target_defaults(${target})

    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    add_test(NAME ${target} COMMAND $<TARGET_FILE:${target}>)
    set_tests_properties(${target} PROPERTIES LABELS "unit")
    list(APPEND GLDIAGRAMDESKTOP_TEST_TARGETS ${target})
    set(GLDIAGRAMDESKTOP_TEST_TARGETS "${GLDIAGRAMDESKTOP_TEST_TARGETS}" PARENT_SCOPE)
endfunction()

set(GLDIAGRAMDESKTOP_TEST_TARGETS "")

# 架构门禁：include/ 中的每个公共函数声明都必须在 src/ 中有定义。
# Python 解释器为必需项：缺失时门禁会静默消失，测试套件仍显示通过。
find_package(Python3 COMPONENTS Interpreter)
if(Python3_Interpreter_FOUND)
    add_test(NAME gldiagramdesktop_decl_def_check
        COMMAND ${Python3_EXECUTABLE}
            ${CMAKE_CURRENT_SOURCE_DIR}/scripts/check_decl_def.py
            ${CMAKE_CURRENT_SOURCE_DIR}
            --min-declarations 1
            --min-definitions 1)
    set_tests_properties(gldiagramdesktop_decl_def_check PROPERTIES LABELS "architecture")
else()
    message(FATAL_ERROR
        "未找到 Python3 解释器，但 GLDIAGRAMDESKTOP_BUILD_TESTING 为 ON，"
        "架构门禁需要它。请安装 Python 3，或使用 "
        "-DGLDIAGRAMDESKTOP_BUILD_TESTING=OFF 关闭测试。")
endif()

# 门禁自检：确认质量门禁脚本确实能够失败。
add_test(NAME gldiagramdesktop_gate_behaviors
    COMMAND ${Python3_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/scripts/tests/test_gate_behaviors.py)
set_tests_properties(gldiagramdesktop_gate_behaviors PROPERTIES LABELS "architecture")

gldiagramdesktop_add_test(gldiagramdesktop_version_tests
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_version.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gldiagramdesktop/version.c
)

# 版本门禁：CMake project VERSION、生成头文件、CHANGELOG 与 --version 输出一致。
set(GLDIAGRAMDESKTOP_VERSION_GATE_ARGS
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/check_version_consistency.py
    ${CMAKE_CURRENT_SOURCE_DIR}
    --build-dir ${CMAKE_BINARY_DIR}
)
if(TARGET gldiagramdesktop)
    list(APPEND GLDIAGRAMDESKTOP_VERSION_GATE_ARGS --exe $<TARGET_FILE:gldiagramdesktop>)
endif()
add_test(NAME gldiagramdesktop_version_consistency
    COMMAND ${Python3_EXECUTABLE} ${GLDIAGRAMDESKTOP_VERSION_GATE_ARGS}
)
set_tests_properties(gldiagramdesktop_version_consistency PROPERTIES LABELS "architecture")
if(TARGET gldiagramdesktop)
    set_tests_properties(gldiagramdesktop_version_consistency PROPERTIES DEPENDS gldiagramdesktop)
endif()

# 文档门禁：链接、锚点、代码围栏与 README 索引覆盖。
add_test(NAME gldiagramdesktop_docs_check
    COMMAND ${Python3_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/scripts/check_docs.py
        ${CMAKE_CURRENT_SOURCE_DIR}
        --mode all)
set_tests_properties(gldiagramdesktop_docs_check PROPERTIES LABELS "documentation")

if(GLDIAGRAMDESKTOP_INSTALL)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
    add_test(NAME gldiagramdesktop_cpack_config
        COMMAND ${Python3_EXECUTABLE}
            ${CMAKE_CURRENT_SOURCE_DIR}/scripts/check-cpack-config.py
            ${CMAKE_CURRENT_BINARY_DIR}/CPackConfig.cmake
    )
endif()

add_custom_target(gldiagramdesktop_tests
    DEPENDS
        ${GLDIAGRAMDESKTOP_TEST_TARGETS}
)
