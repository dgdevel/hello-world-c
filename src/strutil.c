/* strutil.c - small byte-oriented string helpers, no locale deps. */
#include "hello.h"

#include <ctype.h>
#include <string.h>

char *str_trim(char *s) {
    char *start = s;
    /* find first non-space */
    while (*start && isspace((unsigned char)*start)) start++;
    if (start != s) memmove(s, start, strlen(start) + 1);
    /* strip trailing space */
    {
        size_t len = strlen(s);
        while (len > 0 && isspace((unsigned char)s[len - 1])) s[--len] = '\0';
    }
    return s;
}

void str_reverse_n(char *s, size_t n) {
    size_t i = 0, j = n;
    /* plain byte swap; NULs and utf-8 lead bytes pass through untouched */
    while (i + 1 < j--) {
        char t = s[i];
        s[i] = s[j];
        s[j] = t;
        i++;
    }
}

int str_starts(const char *s, const char *pfx) {
    size_t n = strlen(pfx);
    return strncmp(s, pfx, n) == 0;
}

void str_upper_ascii(char *s) {
    for (; *s; s++)
        if (*s >= 'a' && *s <= 'z') *s = (char)(*s - 'a' + 'A');
}
