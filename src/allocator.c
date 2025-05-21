#include "../include/allocator.h"
#include <stdlib.h>

u8* AllocatorRawAlloc(Allocator allocator, i64 size, i64 count, i64 align) {
    return allocator.vtable->alloc(allocator.ctx, size, count, align);
}

void AllocatorRawFree(Allocator allocator, u8* ptr, i64 size, i64 count, i64 align) {
    allocator.vtable->free(allocator.ctx, ptr, size, count, align);
}

static u8* StdAlloc(void* ctx, i64 size, i64 count, i64 align) {
    (void)ctx;
    (void)align;
    
    return (u8*)calloc(count, size);
}

static void StdFree(void* ctx, u8* ptr, i64 size, i64 count, i64 align) {
    (void)ctx;
    (void)size;
    (void)count;
    (void)align;
    free(ptr);
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

