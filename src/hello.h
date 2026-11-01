/* hello.h - single public header: buffer, string utils, cli parser,
   update/crash-report decls. one include for the whole src/ tree. */
#ifndef HELLO_H
#define HELLO_H

#include <stddef.h>

/* release builds stamp this: tools/release.sh passes VERSION=x.y.z to make */
#ifndef HELLO_VERSION
#define HELLO_VERSION "0.0.1"
#endif

/* growable byte buffer. always NUL-terminates so buf.data doubles as a
   c string when the content has no embedded NULs. */
typedef struct {
    char *data;
    size_t len;
    size_t cap;
} buf_t;

void buf_init(buf_t *b);
void buf_free(buf_t *b);
/* append n bytes; returns 0 ok, -1 oom (buffer stays valid) */
int buf_append(buf_t *b, const void *data, size_t n);
/* printf-style append: vsnprintf(NULL,0) for the size, grow, vsnprintf
   again (two-pass). returns 0 ok, -1 oom or format error */
int buf_appendf(buf_t *b, const char *fmt, ...);
void buf_reset(buf_t *b);

/* string utils (strutil.c), all byte-oriented and locale-unsafe by design */
/* trim leading+trailing isspace in place, returns s */
char *str_trim(char *s);
/* in-place byte reverse of the first n bytes; NUL-unaware so fuzz-safe */
void str_reverse_n(char *s, size_t n);
/* 1 if s starts with pfx */
int str_starts(const char *s, const char *pfx);
/* uppercase a-z in place, other bytes untouched */
void str_upper_ascii(char *s);

/* cli parser (cli.c). long options only: --name, --name=value.
   a lone "--" ends options; everything after is positional. */
struct cli_opt {
    const char *name;
    int has_arg; /* 0 flag, 1 value */
};

struct cli_args {
    /* malloc'd array of positional args, caller frees (the strings
       themselves are borrowed from argv) */
    const char *const *pos;
    int npos;
    /* value of the last option seen that took one (from --name <v> or
       --name=v); NULL when none was seen */
    const char *name_value;
    /* bit0: -h/--help seen */
    int flags;
};

/* returns 0 ok, -1 unknown option, -2 missing argument; errors go to
   stderr. on oom writes to stderr and returns -1. */
int cli_parse(int argc, char **argv, const struct cli_opt *opts,
              size_t nopts, struct cli_args *out);

/* crash report (crash_report.c) */
void crash_report_init(const char *argv0);

/* update (update.c) */
int update_run(int argc, char **argv);
const char *update_current_version(void);

/* semver parser exposed for tests: "v1.2.3", "1.2", "v3" parse ok;
   shorter prefixes get zeros; junk fails with ok=0 */
typedef struct {
    int maj, min, pat;
    int ok;
} hello_semver_t;
hello_semver_t update_parse_ver(const char *tag);

#endif
