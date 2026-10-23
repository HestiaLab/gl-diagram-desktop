#include "platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#else
#include <unistd.h>
#endif

#define GLDIAGRAMDESKTOP_FONT_RELATIVE "fonts/NotoSansSC-Regular.ttf"

static int file_exists(const char *path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        return 0;
    }
    (void)fclose(file);
    return 1;
}

static int executable_directory(char *buffer, size_t size) {
#if defined(_WIN32)
    DWORD length = GetModuleFileNameA(NULL, buffer, (DWORD)size);
    if (length == 0 || (size_t)length >= size) {
        return -1;
    }
#elif defined(__APPLE__)
    uint32_t length = (uint32_t)size;
    if (_NSGetExecutablePath(buffer, &length) != 0) {
        return -1;
    }
#else
    ssize_t length = readlink("/proc/self/exe", buffer, size - 1);
    if (length <= 0 || (size_t)length >= size - 1) {
        return -1;
    }
    buffer[length] = '\0';
#endif

    char *separator = strrchr(buffer, '/');
#if defined(_WIN32)
    char *backslash = strrchr(buffer, '\\');
    if (backslash != NULL && (separator == NULL || backslash > separator)) {
        separator = backslash;
    }
#endif
    if (separator == NULL) {
        return -1;
    }
    *separator = '\0';
    return 0;
}

int gldiagramdesktop_font_path(char *buffer, size_t size) {
    if (buffer == NULL || size == 0) {
        return -1;
    }

    const char *override_path = getenv("GLDIAGRAMDESKTOP_FONT");
    if (override_path != NULL && override_path[0] != '\0' && file_exists(override_path)) {
        int written = snprintf(buffer, size, "%s", override_path);
        return (written > 0 && (size_t)written < size) ? 0 : -1;
    }

    char directory[GLDIAGRAMDESKTOP_PATH_MAX];
    if (executable_directory(directory, sizeof(directory)) != 0) {
        return -1;
    }

    int written = snprintf(buffer, size, "%s/%s", directory, GLDIAGRAMDESKTOP_FONT_RELATIVE);
    if (written <= 0 || (size_t)written >= size) {
        return -1;
    }
    return file_exists(buffer) ? 0 : -1;
}
