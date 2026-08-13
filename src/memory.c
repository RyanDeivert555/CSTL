#include "CSTL/memory.h"
#include "CSTL/common.h"
#include <string.h>

void cstl_raw_memcpy(void* restrict dest, const void* restrict src, i64 size, i64 count) {
    cstl_assert(size >= 0 && count >= 0);

    if (count == 0) {
        return;
    }

    memcpy(dest, src, size * count);
}

void cstl_raw_memmove(void* dest, const void* src, i64 size, i64 count) {
    cstl_assert(size >= 0 && count >= 0);

    if (count == 0) {
        return;
    }

    memmove(dest, src, size * count);
}

bool cstl_raw_memcmp(const void* p0, const void* p1, i64 size, i64 count) {
    cstl_assert(size >= 0 && count >= 0);

    if (count == 0) {
        return true;
    } else {
        return memcmp(p0, p1, size * count) == 0;
    }
}
