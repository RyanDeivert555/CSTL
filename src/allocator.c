#include "CSTL/allocator.h"
#include <stdlib.h>

u8* allocator_raw_alloc(allocator allocator, i64 size, i64 count, i64 align) {
    return allocator.vtable->alloc(allocator.ctx, size, count, align);
}

void allocator_raw_free(allocator allocator, u8* ptr, i64 size, i64 count, i64 align) {
    allocator.vtable->free(allocator.ctx, ptr, size, count, align);
}

static u8* std_alloc(void* ctx, i64 size, i64 count, i64 align) {
    (void)ctx;

#if defined(_WIN32)
    return (u8*)_aligned_malloc(size * count, align);
#else
    return (u8*)aligned_alloc(align, size * count);
#endif
}

static void std_free(void* ctx, u8* ptr, i64 size, i64 count, i64 align) {
    (void)ctx;
    (void)size;
    (void)count;
    (void)align;

#if defined(_WIN32)
    _aligned_free(ptr);
#else
    free(ptr);
#endif
}

static const allocator_v_table std_allocator_vtable = {
    .alloc = std_alloc,
    .free = std_free,
};

allocator std_allocator(void) {
    const allocator result = {
        .ctx = NULL,
        .vtable = &std_allocator_vtable,
    };

    return result;
}
