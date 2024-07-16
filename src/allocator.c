#include "../include/allocator.h"
#include <stdlib.h>
#include <assert.h>

void* allocator_alloc(allocator allocator, size_t elem_size, size_t size) {
    return allocator.vtable->alloc(allocator.ctx, elem_size, size);
}

void* allocator_realloc(allocator allocator, void* ptr, size_t elem_size, size_t size) {
    return allocator.vtable->realloc(allocator.ctx, ptr, elem_size, size);
}

void allocator_free(allocator allocator, void* ptr, size_t elem_size, size_t size) {
    allocator.vtable->free(allocator.ctx, ptr, elem_size, size);
}

static void* c_alloc(void* ctx, size_t elem_size, size_t size) {
    (void)ctx;
    
    return malloc(elem_size * size);
}

static void* c_realloc(void* ctx, void* ptr, size_t elem_size, size_t size) {
    (void)ctx;

    return realloc(ptr, elem_size * size);
}

static void c_free(void* ctx, void* ptr, size_t elem_size, size_t size) {
    (void)ctx;
    (void)elem_size;
    (void)size;
    free(ptr);
}

static allocator_vtable c_allocator_vtable = {
    .alloc = c_alloc,
    .realloc = c_realloc,
    .free = c_free,
};

allocator c_allocator(void) {
    allocator result = {
        .ctx = NULL,
        .vtable = &c_allocator_vtable,
    };

    return result;
}
