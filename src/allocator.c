#include "../include/allocator.h"
#include <stdlib.h>

void* allocator_alloc(allocator allocator, size_t size) {
    return allocator.vtable.alloc(allocator.ctx, size);
}

void* allocator_realloc(allocator allocator, void* ptr, size_t size) {
    return allocator.vtable.realloc(allocator.ctx, ptr, size);
}

void allocator_free(allocator allocator, void* ptr) {
    allocator.vtable.free(allocator.ctx, ptr);
}

void* basic_alloc(void* _, size_t size) {
    // TODO: should this use calloc?
    return malloc(size);
}

void* basic_realloc(void* _, void* ptr, size_t size) {
    return realloc(ptr, size);
}

void basic_free(void* _, void* ptr) {
    free(ptr);
}

allocator basic_allocator(void) {
    allocator result = {
        .ctx = NULL,
        .vtable = {
            .alloc = basic_alloc,
            .realloc = basic_realloc,
            .free = basic_free,
        },
    };

    return result;
}
