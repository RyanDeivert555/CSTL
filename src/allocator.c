#include "CSTL/allocator.h"
#include <stdlib.h>

bool cstl_is_pow2(isize x) {
    return x > 0 && (x & (x - 1)) == 0;
}

isize cstl_next_pow2(isize n) {
    if (n <= 1) {
        return 1;
    }

    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;

    return n + 1;
}

cstl_byte* cstl_raw_alloc(allocator a, isize size, isize count, isize align) {
    if (size <= 0 || count <= 0 || !cstl_is_pow2(align)) {
        return NULL;
    }
    if (size > ISIZE_MAX / count) {
        return NULL;
    }

    return a.vtable->alloc(a.ctx, size, count, align);
}

void cstl_raw_free(allocator a, cstl_byte* ptr, isize size, isize count, isize align) {
    a.vtable->free(a.ctx, ptr, size, count, align);
}

static cstl_byte* std_alloc(void* ctx, isize size, isize count, isize align) {
    cstl_unused(ctx);

    const isize total = size * count;

    if (align <= (isize)alignof(max_align_t)) {
        return (cstl_byte*)malloc(total);
    } else {
#if defined(_WIN32)
        return (cstl_byte*)_aligned_malloc(total, align);
#else
        const isize aligned_total = (total + align - 1) & ~(align - 1);

        return (cstl_byte*)aligned_alloc(align, aligned_total);
#endif
    }
}

static void std_free(void* ctx, cstl_byte* ptr, isize size, isize count, isize align) {
    cstl_unused(ctx);
    cstl_unused(size);
    cstl_unused(count);

#if defined(_WIN32)
    if (align > (isize)alignof(max_align_t)) {
        _aligned_free(ptr);
        return;
    } else {
        free(ptr);
    }
#else
    cstl_unused(align);

    free(ptr);
#endif
}

static const allocator_vtable vtable = {
    .alloc = std_alloc,
    .free = std_free,
};

allocator std_allocator(void) {
    return (allocator){
        .ctx = NULL,
        .vtable = &vtable,
    };
}
