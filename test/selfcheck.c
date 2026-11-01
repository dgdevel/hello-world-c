/* selfcheck.c - self-contained test harness, no framework. exits 1 on
   any failure so make check can gate on it. */
#include "hello.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int tests_run, tests_failed;

/* one check = one line: prints ok/fail and counts both */
static void check(int cond, const char *name) {
    tests_run++;
    if (cond) {
        printf("  [ ok ] %s\n", name);
    } else {
        tests_failed++;
        printf("  [FAIL] %s\n", name);
    }
}

static void check_int(int got, int want, const char *name) {
    tests_run++;
    if (got == want) {
        printf("  [ ok ] %s\n", name);
    } else {
        tests_failed++;
        printf("  [FAIL] %s: got %d want %d\n", name, got, want);
    }
}

static void check_str(const char *got, const char *want, const char *name) {
    tests_run++;
    if (got && want && !strcmp(got, want)) {
        printf("  [ ok ] %s\n", name);
    } else {
        tests_failed++;
        printf("  [FAIL] %s: got '%s' want '%s'\n",
               name, got ? got : "(null)", want ? want : "(null)");
    }
}

static void test_buf(void) {
    buf_t b;
    char big[65536];
    memset(big, 'x', sizeof big);
    printf("buf:\n");
    buf_init(&b);
    check_int((int)b.len, 0, "buf_init len 0");
    check(b.data == NULL, "buf_init data null");
    check_int(buf_append(&b, "hello", 5), 0, "buf_append hello");
    check_int(buf_append(&b, " world", 6), 0, "buf_append world");
    check_str(b.data, "hello world", "buf_append content");
    check_int((int)b.len, 11, "buf_append len");
    check_int(buf_appendf(&b, "%d-%s", 42, "x"), 0, "buf_appendf");
    check_str(b.data, "hello world42-x", "buf_appendf content");
    buf_reset(&b);
    check_int((int)b.len, 0, "buf_reset len 0");
    check(b.data && b.data[0] == '\0', "buf_reset nul");
    /* big append crosses the doubling path without oom */
    check_int(buf_append(&b, big, sizeof big), 0, "buf_append 64k");
    check_int((int)b.len, (int)sizeof big, "buf_append 64k len");
    check(b.data[b.len] == '\0', "nul kept after append");
    buf_free(&b);
    check(b.data == NULL, "buf_free nulls data");
}

static void test_strutil(void) {
    char t1[] = "  hi  ";
    char t2[] = "";
    char t3[] = "\t\n mid \r\n";
    char r1[] = "abc";
    char r2[] = "ab";
    char u1[] = "Hello, World 123!";
    char empty[] = "";
    printf("strutil:\n");
    check_str(str_trim(t1), "hi", "str_trim spaces");
    check_str(str_trim(t2), "", "str_trim empty");
    check_str(str_trim(t3), "mid", "str_trim mixed space");
    str_reverse_n(r1, 3);
    check_str(r1, "cba", "str_reverse_n abc");
    str_reverse_n(r2, 2);
    check_str(r2, "ba", "str_reverse_n ab");
    str_reverse_n(empty, 0);
    check_str(empty, "", "str_reverse_n n=0");
    check_int(str_starts("hello", "hel"), 1, "str_starts yes");
    check_int(str_starts("he", "hello"), 0, "str_starts short");
    check_int(str_starts("hello", ""), 1, "str_starts empty pfx");
    str_upper_ascii(u1);
    check_str(u1, "HELLO, WORLD 123!", "str_upper_ascii");
}

static void test_cli(void) {
    struct cli_opt opts[] = {{"verbose", 0}, {"name", 1}};
    struct cli_args out;
    char *a1[] = {"prog", "--verbose", "--name", "val", "pos",
                  "--name=other", NULL};
    char *a2[] = {"prog", "--help", NULL};
    char *a3[] = {"prog", "--nope", NULL};
    char *a4[] = {"prog", "--name", NULL};
    char *a5[] = {"prog", "a", "--", "--notopt", "b", NULL};
    printf("cli:\n");
    check_int(cli_parse(6, a1, opts, 2, &out), 0, "cli_parse ok");
    check_int(out.flags & 1, 0, "help bit clear");
    check_str(out.name_value, "other", "name_value last wins");
    check_int(out.npos, 1, "npos 1");
    check_str(out.pos[0], "pos", "pos[0]");
    free((void *)out.pos);
    check_int(cli_parse(2, a2, opts, 2, &out), 0, "cli_parse --help");
    check_int(out.flags & 1, 1, "help bit set");
    free((void *)out.pos);
    check_int(cli_parse(2, a3, opts, 2, &out), -1, "unknown option -1");
    check_int(cli_parse(2, a4, opts, 2, &out), -2, "missing value -2");
    check_int(cli_parse(5, a5, opts, 2, &out), 0, "cli_parse -- marker");
    check_int(out.npos, 3, "post-marker positionals");
    check_str(out.pos[1], "--notopt", "-- becomes positional");
    free((void *)out.pos);
}

static void test_semver(void) {
    hello_semver_t v;
    printf("update_parse_ver:\n");
    v = update_parse_ver("v1.2.3");
    check_int(v.ok, 1, "v1.2.3 ok");
    check(v.ok && v.maj == 1 && v.min == 2 && v.pat == 3, "v1.2.3 fields");
    v = update_parse_ver("1.2");
    check(v.ok && v.maj == 1 && v.min == 2 && v.pat == 0, "1.2 -> 1.2.0");
    v = update_parse_ver("v3");
    check(v.ok && v.maj == 3 && v.min == 0 && v.pat == 0, "v3 -> 3.0.0");
    v = update_parse_ver("x9");
    check_int(v.ok, 0, "x9 junk");
    v = update_parse_ver("");
    check_int(v.ok, 0, "empty junk");
    v = update_parse_ver("1.2.x");
    check_int(v.ok, 0, "1.2.x junk");
}

int main(void) {
    test_buf();
    test_strutil();
    test_cli();
    test_semver();
    printf("selfcheck: %d passed, %d failed\n", tests_run - tests_failed,
           tests_failed);
    return tests_failed ? 1 : 0;
}
