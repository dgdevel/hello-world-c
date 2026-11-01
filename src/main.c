/* main.c - subcommand dispatch, help, version. */
#include "hello.h"
#include "crash_report.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

static int usage(FILE *out) {
    fputs("usage: hello-world-c <command> [args]\n"
          "\n"
          "commands:\n"
          "  hello [name]           print a greeting\n"
          "  echo <text...>         echo text; - reads stdin\n"
          "  reverse <text...>      reverse text; - reads stdin\n"
          "  wc                     count lines/words/bytes of stdin\n"
          "  update [--yes]         check github for a newer release and\n"
          "                         install it\n"
          "  help                   show this help\n"
          "  version                show the version\n",
          out);
    return 1;
}

static int cmd_help(void) {
    usage(stdout);
    return 0;
}

static int cmd_version(void) {
    printf("hello-world-c %s\n", HELLO_VERSION);
    return 0;
}

static int cmd_hello(int argc, char **argv) {
    if (argc == 2) {
        printf("hello, world!\n");
        return 0;
    }
    if (argc == 3) {
        printf("hello, %s!\n", argv[2]);
        return 0;
    }
    usage(stderr);
    return 1;
}

static int cmd_echo(int argc, char **argv) {
    int c;
    if (argc < 3 || !strcmp(argv[2], "-")) {
        /* stdin passthrough, byte-wise */
        while ((c = fgetc(stdin)) != EOF) fputc(c, stdout);
        return ferror(stdin) ? 1 : 0;
    }
    for (c = 2; c < argc; c++) {
        fputs(argv[c], stdout);
        if (c + 1 < argc) fputc(' ', stdout);
    }
    fputc('\n', stdout);
    return 0;
}

static int cmd_reverse(int argc, char **argv) {
    buf_t b;
    buf_init(&b);
    if (argc < 3 || !strcmp(argv[2], "-")) {
        /* slurp stdin whole, reverse all bytes including newlines */
        int c;
        while ((c = fgetc(stdin)) != EOF)
            if (buf_append(&b, &(char){(char)c}, 1) != 0) {
                buf_free(&b);
                return 1;
            }
    } else {
        /* join args with spaces, reverse the one line */
        int i;
        for (i = 2; i < argc; i++) {
            if (i > 2 && buf_append(&b, " ", 1) != 0) {
                buf_free(&b);
                return 1;
            }
            if (buf_append(&b, argv[i], strlen(argv[i])) != 0) {
                buf_free(&b);
                return 1;
            }
        }
    }
    str_reverse_n(b.data, b.len);
    fwrite(b.data, 1, b.len, stdout);
    /* joined args print as one line; stdin keeps its own bytes */
    if (argc >= 3 && strcmp(argv[2], "-") != 0) fputc('\n', stdout);
    buf_free(&b);
    return ferror(stdout) ? 1 : 0;
}

static int cmd_wc(void) {
    int lines = 0, words = 0, bytes = 0;
    int c, prev_space = 1;
    while ((c = fgetc(stdin)) != EOF) {
        bytes++;
        if (c == '\n') lines++;
        if (isspace(c)) {
            prev_space = 1;
        } else {
            /* word = isspace-delimited run, counted at its first byte */
            if (prev_space) words++;
            prev_space = 0;
        }
    }
    printf("%d %d %d\n", lines, words, bytes);
    return ferror(stdin) ? 1 : 0;
}

static int cmd_update(int argc, char **argv) {
    return update_run(argc, argv);
}

int main(int argc, char **argv) {
    const char *cmd;
    crash_report_init(argc > 0 && argv[0] ? argv[0] : "hello-world-c");
    if (argc < 2) return usage(stderr);
    cmd = argv[1];
    if (!strcmp(cmd, "hello")) return cmd_hello(argc, argv);
    if (!strcmp(cmd, "echo")) return cmd_echo(argc, argv);
    if (!strcmp(cmd, "reverse")) return cmd_reverse(argc, argv);
    if (!strcmp(cmd, "wc")) return cmd_wc();
    if (!strcmp(cmd, "update")) return cmd_update(argc, argv);
    if (!strcmp(cmd, "help")) return cmd_help();
    if (!strcmp(cmd, "version")) return cmd_version();
    fprintf(stderr, "hello-world-c: unknown command '%s'\n", cmd);
    return usage(stderr);
}
