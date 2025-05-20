#include "../include/allocator.h"
#include <stdlib.h>

u8* allocator_raw_alloc(allocator allocator, i64 size, i64 count, i64 align) {
    return allocator.vtable->alloc(allocator.ctx, size, count, align);
}

void allocator_raw_free(allocator allocator, u8* ptr, i64 size, i64 count, i64 align) {
    allocator.vtable->free(allocator.ctx, ptr, size, count, align);
}

static u8* c_alloc(void* ctx, i64 size, i64 count, i64 align) {
    (void)ctx;
    (void)align;
    
    return (u8*)calloc(count, size);
}

static void c_free(void* ctx, u8* ptr, i64 size, i64 count, i64 align) {
    (void)ctx;
    (void)size;
    (void)count;
    (void)align;
    free(ptr);
}

static const allocator_vtable c_allocator_vtable = {
    .alloc = c_alloc,
    .free = c_free,
};

allocator c_allocator(void) {
    const allocator result = {
        .ctx = NULL,
        .vtable = &c_allocator_vtable,
    };

    return result;
}

