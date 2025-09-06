#include "allocator.h"
#include <stdlib.h>

u8* AllocatorRawAlloc(Allocator allocator, i64 size, i64 count, i64 align) {
    return allocator.vtable->alloc(allocator.ctx, size, count, align);
}

void AllocatorRawFree(Allocator allocator, u8* ptr, i64 size, i64 count, i64 align) {
    allocator.vtable->free(allocator.ctx, ptr, size, count, align);
}

static u8* PortableAlignedAlloc(i64 size, i64 count, i64 align) {
#if defined(_WIN32)
    return (u8*)_aligned_malloc(size * count, align);
#else
    return (u8*)aligned_alloc(align, size * count);
#endif
}

static void PortableFree(u8* ptr) {
#if defined(_WIN32)
    _aligned_free(ptr);
#else
    free(ptr);
#endif
}

static u8* StdAlloc(void* ctx, i64 size, i64 count, i64 align) {
    (void)ctx;

    return PortableAlignedAlloc(size, count, align);
}

static void StdFree(void* ctx, u8* ptr, i64 size, i64 count, i64 align) {
    (void)ctx;
    (void)size;
    (void)count;
    (void)align;
    PortableFree(ptr);
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

