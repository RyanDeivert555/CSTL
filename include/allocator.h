#pragma once
#include <stddef.h>

typedef struct allocator_vtable {
    void* (*alloc)(void* ctx, size_t elem_size, size_t size);
    void* (*realloc)(void* vtx, void* ptr, size_t elem_size, size_t size);
    void (*free)(void* ctx, void* ptr, size_t elem_size, size_t size);
} allocator_vtable;

typedef struct allocator {
    void* ctx;
    allocator_vtable vtable;
} allocator;

void* allocator_alloc(allocator allocator, size_t elem_size, size_t size);
void* allocator_realloc(allocator allocator, void* ptr, size_t elem_size, size_t size);
void allocator_free(allocator allocator, void* ptr, size_t elem_size, size_t size);

allocator basic_allocator(void);
