#include "CSTL/allocator.h"
#include <stdlib.h>

U8* AllocatorRawAlloc(Allocator allocator, I64 size, I64 count, I64 align) {
    return allocator.vtable->alloc(allocator.ctx, size, count, align);
}

void AllocatorRawFree(Allocator allocator, U8* ptr, I64 size, I64 count, I64 align) {
    allocator.vtable->free(allocator.ctx, ptr, size, count, align);
}

static U8* StdAlloc(void* ctx, I64 size, I64 count, I64 align) {
    (void)ctx;

#if defined(_WIN32)
    return (U8*)_aligned_malloc(size * count, align);
#else
    return (U8*)aligned_alloc(align, size * count);
#endif
}

static void StdFree(void* ctx, U8* ptr, I64 size, I64 count, I64 align) {
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

static const AllocatorVTable std_allocator_vtable = {
    .alloc = StdAlloc,
    .free = StdFree,
};

Allocator StdAllocator(void) {
    const Allocator result = {
        .ctx = NULL,
        .vtable = &std_allocator_vtable,
    };

    return result;
}
