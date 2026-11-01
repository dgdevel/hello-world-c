/* fuzz/fuzz_reverse.c - str_reverse_n byte invariants on a scratch
 * copy. the input may contain NULs; str_reverse_n is NUL-unaware by
 * design, so plain memcpy + reverse must round-trip every byte. */
#include "hello.h"

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    uint8_t *scratch;
    size_t i;

    if (size == 0) return 0;
    scratch = malloc(size);
    if (!scratch) return 0;
    memcpy(scratch, data, size);

    str_reverse_n((char *)scratch, size);

    /* scratch[i] must be data[size-1-i] for every i; abort() gives the
       fuzzer a real target instead of silent corruption */
    for (i = 0; i < size; i++)
        if (scratch[i] != data[size - 1 - i]) abort();

    free(scratch);
    return 0;
}
