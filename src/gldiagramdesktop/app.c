#include <gldiagramdesktop/app.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING

#include <nuklear/nuklear.h>
#include <nuklear/nuklear_glfw_gl3.h>

#include "platform.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define UI_FONT_HEIGHT 18.0f
#define MAX_VERTEX_BUFFER (512 * 1024)
#define MAX_ELEMENT_BUFFER (128 * 1024)

static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}

static struct nk_font *load_ui_font(struct nk_font_atlas *atlas, char *font_path,
                                    size_t path_size) {
    if (gldiagramdesktop_font_path(font_path, path_size) != 0) {
        return NULL;
    }

    struct nk_font_config config = nk_font_config(UI_FONT_HEIGHT);
    config.range = nk_font_chinese_glyph_ranges();
    config.oversample_h = 1;
    config.oversample_v = 1;
    config.fallback_glyph = (nk_rune)'?';

    return nk_font_atlas_add_from_file(atlas, font_path, UI_FONT_HEIGHT, &config);
}

static void report_font_failure(void) {
    fputs("界面字体加载失败。\n"
          "请确认可执行文件同级存在 fonts/NotoSansSC-Regular.ttf，\n"
          "或通过 GLDIAGRAMDESKTOP_FONT 环境变量指定字体文件。\n",
          stderr);
}

int gldiagramdesktop_run(void) {
    if (!glfwInit()) {
        fputs("GLFW 初始化失败。\n", stderr);
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    char title[128] = {0};
    (void)snprintf(title, sizeof(title), "GlDiagramDesktop %s - 绘图画布",
                   gldiagramdesktop_version());

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, NULL, NULL);
    if (!window) {
        fputs("窗口创建失败。\n", stderr);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fputs("GLAD 初始化失败。\n", stderr);
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    const GLubyte *gl_version = glGetString(GL_VERSION);
    printf("OpenGL 版本：%s\n", gl_version != NULL ? (const char *)gl_version : "未知");

    int window_width = 0;
    int window_height = 0;
    int framebuffer_width = 0;
    int framebuffer_height = 0;
    glfwGetWindowSize(window, &window_width, &window_height);
    glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
    printf("窗口：%dx%d，帧缓冲：%dx%d\n", window_width, window_height, framebuffer_width,
           framebuffer_height);
    (void)fflush(stdout);

    struct nk_glfw glfw_ctx = {0};
    struct nk_context *ctx = nk_glfw3_init(&glfw_ctx, window, NK_GLFW3_INSTALL_CALLBACKS);
    if (ctx == NULL) {
        fputs("Nuklear GLFW 后端初始化失败。\n", stderr);
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    char font_path[GLDIAGRAMDESKTOP_PATH_MAX] = {0};
    struct nk_font_atlas *atlas = NULL;
    nk_glfw3_font_stash_begin(&glfw_ctx, &atlas);

    struct nk_font *ui_font = load_ui_font(atlas, font_path, sizeof(font_path));
    if (ui_font == NULL) {
        report_font_failure();
        nk_glfw3_shutdown(&glfw_ctx);
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    nk_glfw3_font_stash_end(&glfw_ctx);
    nk_style_set_font(ctx, &ui_font->handle);
    printf("界面字体已加载：%s\n", font_path);
    (void)fflush(stdout);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        nk_glfw3_new_frame(&glfw_ctx);

        if (nk_begin(ctx, "显示", nk_rect(50.0f, 50.0f, 220.0f, 220.0f),
                     NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE)) {
            nk_layout_row_static(ctx, 30, 100, 1);

            if (nk_button_label(ctx, "点击我")) {
                printf("按钮已点击\n");
                (void)fflush(stdout);
            }
        }

        nk_end(ctx);

        glClear(GL_COLOR_BUFFER_BIT);

        nk_glfw3_render(&glfw_ctx, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);

        glfwSwapBuffers(window);
    }

    nk_glfw3_shutdown(&glfw_ctx);

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
