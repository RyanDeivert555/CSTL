#include "../include/allocator.h"
#include <stdlib.h>
#include <assert.h>

void* allocator_alloc(allocator allocator, size_t size) {
    return allocator.vtable.alloc(allocator.ctx, size);
}

mem_slice allocator_alloc_slice(allocator allocator, size_t elem_size, size_t size) {
    return allocator.vtable.alloc_slice(allocator.ctx, elem_size, size);
}

void* allocator_realloc(allocator allocator, void* ptr, size_t size) {
    return allocator.vtable.realloc(allocator.ctx, ptr, size);
}

mem_slice allocator_realloc_slice(allocator allocator, mem_slice slice, size_t elem_size, size_t size) {
    return allocator.vtable.realloc_slice(allocator.ctx, slice, elem_size, size);
}

void allocator_free(allocator allocator, void* ptr) {
    allocator.vtable.free(allocator.ctx, ptr);
}

void allocator_free_slice(allocator allocator, mem_slice slice) {
    allocator.vtable.free_slice(allocator.ctx, slice);
}

void* basic_alloc(void* _, size_t size) {
    // TODO: should this use calloc?
    return malloc(size);
}

mem_slice basic_alloc_slice(void *_, size_t elem_size, size_t size) {
    mem_slice result = {
        .ptr = malloc(elem_size * size),
        .length = size,
    };

    return result;
}

void* basic_realloc(void* _, void* ptr, size_t size) {
    return realloc(ptr, size);
}

mem_slice basic_realloc_slice(void* _, mem_slice slice, size_t elem_size, size_t size) {
    mem_slice result = {
        .ptr = realloc(slice.ptr, elem_size * size),
        .length = size,
    };

    return result;
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
