#include "CSTL/allocator.h"
#include <stdlib.h>

static bool is_pow2(i64 x) {
    return x > 0 && (x & (x - 1)) == 0;
}

u8* cstl_raw_alloc(allocator a, i64 size, i64 count, i64 align) {
    if (count <= 0 || size <= 0 || !is_pow2(align)) {
        return NULL;
    }
    if (count > I64_MAX / size) {
        return NULL;
    }

    return a.vtable->alloc(a.ctx, size, count, align);
}

void cstl_raw_free(allocator a, u8* ptr, i64 size, i64 count, i64 align) {
    a.vtable->free(a.ctx, ptr, size, count, align);
}

static u8* std_alloc(void* ctx, i64 size, i64 count, i64 align) {
    cstl_unused(ctx);

    const i64 total = size * count;

    if (align <= (i64)alignof(max_align_t)) {
        return (u8*)malloc(total);
    } else {
#if defined(_WIN32)
        return (u8*)_aligned_malloc(total, align);
#else
        const i64 aligned_total = (total + align - 1) & ~(align - 1);

        return (u8*)aligned_alloc(align, aligned_total);
#endif
    }
}

static void std_free(void* ctx, u8* ptr, i64 size, i64 count, i64 align) {
    cstl_unused(ctx);
    cstl_unused(size);
    cstl_unused(count);

#if defined(_WIN32)
    if (align > (i64)alignof(max_align_t)) {
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
