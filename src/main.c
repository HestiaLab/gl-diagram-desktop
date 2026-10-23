#include <gldiagramdesktop/app.h>

#include <stdio.h>
#include <string.h>

static void print_usage(FILE *stream, const char *program) {
    fprintf(stream,
            "Usage: %s [options]\n"
            "\n"
            "Options:\n"
            "  -h, --help     Show this help\n"
            "  -v, --version  Show version\n",
            program);
}

int main(int argc, char **argv) {
    const char *program = (argc > 0 && argv[0] != NULL) ? argv[0] : "gldiagramdesktop";

    for (int i = 1; i < argc; ++i) {
        const char *arg = argv[i];
        if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
            print_usage(stdout, program);
            return 0;
        }
        if (strcmp(arg, "-v") == 0 || strcmp(arg, "--version") == 0) {
            printf("%s %s\n", "gldiagramdesktop", gldiagramdesktop_version());
            return 0;
        }
        fprintf(stderr, "Unknown option: %s\n", arg);
        print_usage(stderr, program);
        return 2;
    }

    return gldiagramdesktop_run();
}
