#include "../include/allocator.h"
#include <stdint.h>
#include <stdlib.h>

uint8_t* allocator_raw_alloc(allocator allocator, int64_t size, int64_t count, int64_t align) {
    return allocator.vtable->alloc(allocator.ctx, size, count, align);
}

void allocator_raw_free(allocator allocator, uint8_t* ptr, int64_t size, int64_t count, int64_t align) {
    allocator.vtable->free(allocator.ctx, ptr, size, count, align);
}

static uint8_t* c_alloc(void* ctx, int64_t size, int64_t count, int64_t align) {
    (void)ctx;
    (void)align;
    
    return (uint8_t*)calloc(count, size);
}

static void c_free(void* ctx, uint8_t* ptr, int64_t size, int64_t count, int64_t align) {
    (void)ctx;
    (void)size;
    (void)count;
    (void)align;
    free(ptr);
}

static allocator_vtable c_allocator_vtable = {
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

