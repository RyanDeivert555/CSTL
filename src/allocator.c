#include "CSTL/allocator.h"
#include <stdlib.h>

static void* global_allocator_ctx = NULL;
static alloc_func global_alloc = std_alloc;
static free_func global_free = std_free;

void set_global_allocator(void* ctx, alloc_func alloc, free_func free) {
    global_allocator_ctx = ctx;
    global_alloc = alloc;
    global_free = free;
}

u8* cstl_raw_alloc(i64 size, i64 count, i64 align) {
    return global_alloc(global_allocator_ctx, size, count, align);
}

void cstl_raw_free(u8* ptr, i64 size, i64 count, i64 align) {
    global_free(global_allocator_ctx, ptr, size, count, align);
}

u8* std_alloc(void* ctx, i64 size, i64 count, i64 align) {
    (void)ctx;

#if defined(_WIN32)
    return (u8*)_aligned_malloc(size * count, align);
#else
    // TODO: assert size is multiple of alignment?
    return (u8*)aligned_alloc(align, size * count);
#endif
}

void std_free(void* ctx, u8* ptr, i64 size, i64 count, i64 align) {
    (void)ctx;

#if defined(_WIN32)
    (void)size;
    (void)count;
    (void)align;

    _aligned_free(ptr);
#else
    free_aligned_sized(ptr, align, size * count);
#endif
}
