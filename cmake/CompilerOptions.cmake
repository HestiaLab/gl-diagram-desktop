function(gldiagramdesktop_target_warnings target)
    if(MSVC)
        # 源文件与用户可见文案统一 UTF-8，避免窄字符串被按本地代码页解释。
        target_compile_options(${target} PRIVATE /W4 /permissive- /utf-8)
    else()
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wconversion
        )
    endif()
endfunction()

function(gldiagramdesktop_target_sanitizers target)
    if(GLDIAGRAMDESKTOP_ENABLE_ASAN)
        if(MSVC)
            target_compile_options(${target} PRIVATE /fsanitize=address)
            target_link_options(${target} PRIVATE /fsanitize=address)
        else()
            target_compile_options(${target} PRIVATE -fsanitize=address)
            target_link_options(${target} PRIVATE -fsanitize=address)
        endif()
    endif()

    if(GLDIAGRAMDESKTOP_ENABLE_UBSAN)
        if(NOT MSVC)
            target_compile_options(${target} PRIVATE -fsanitize=undefined)
            target_link_options(${target} PRIVATE -fsanitize=undefined)
        else()
            message(WARNING
                "GLDIAGRAMDESKTOP_ENABLE_UBSAN is ON, but UBSAN is not configured for MSVC in this template.")
        endif()
    endif()
endfunction()

function(gldiagramdesktop_target_coverage target)
    if(NOT GLDIAGRAMDESKTOP_ENABLE_COVERAGE)
        return()
    endif()

    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        target_compile_options(${target} PRIVATE --coverage -O0 -g)
        target_link_options(${target} PRIVATE --coverage)
    else()
        message(WARNING
            "GLDIAGRAMDESKTOP_ENABLE_COVERAGE is ON, but coverage flags are only configured for GCC and Clang-like compilers.")
    endif()
endfunction()

function(gldiagramdesktop_target_defaults target)
    target_compile_features(${target} PRIVATE c_std_11)
    gldiagramdesktop_target_warnings(${target})
    gldiagramdesktop_target_sanitizers(${target})
    gldiagramdesktop_target_coverage(${target})
endfunction()
