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
#define NK_INCLUDE_DEFAULT_FONT

#include <nuklear/nuklear.h>
#include <nuklear/nuklear_glfw_gl3.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MAX_VERTEX_BUFFER (512 * 1024)
#define MAX_ELEMENT_BUFFER (128 * 1024)

static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}

int gldiagramdesktop_run(void) {
    if (!glfwInit()) {
        fputs("Failed to initialize GLFW\n", stderr);
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    char title[128] = {0};
    (void)snprintf(title, sizeof(title), "GlDiagramDesktop %s - Nuklear GUI",
                   gldiagramdesktop_version());

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, NULL, NULL);
    if (!window) {
        fputs("Failed to create window\n", stderr);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fputs("Failed to init GLAD\n", stderr);
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    const GLubyte *version = glGetString(GL_VERSION);
    printf("OpenGL: %s\n", version != NULL ? (const char *)version : "unknown");

    struct nk_glfw glfw_ctx = {0};
    struct nk_context *ctx = nk_glfw3_init(&glfw_ctx, window, NK_GLFW3_INSTALL_CALLBACKS);
    if (ctx == NULL) {
        fputs("Failed to initialize Nuklear GLFW backend\n", stderr);
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    struct nk_font_atlas *atlas = NULL;
    nk_glfw3_font_stash_begin(&glfw_ctx, &atlas);
    nk_glfw3_font_stash_end(&glfw_ctx);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        nk_glfw3_new_frame(&glfw_ctx);

        if (nk_begin(ctx, "Show", nk_rect(50.0f, 50.0f, 220.0f, 220.0f),
                     NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE)) {
            nk_layout_row_static(ctx, 30, 100, 1);

            if (nk_button_label(ctx, "Click Me")) {
                printf("Button clicked\n");
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
