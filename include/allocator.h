#pragma once
#include <stddef.h>

// TODO: see if can reduce size of allocator

typedef struct allocator_vtable {
    void* (*alloc)(void* ctx, size_t size);
    void* (*realloc)(void* ctx, void* ptr, size_t size);
    void (*free)(void* ctx, void* ptr);
} allocator_vtable;

typedef struct allocator {
    void* ctx;
    allocator_vtable vtable;
} allocator;

void* allocator_alloc(allocator allocator, size_t size);
void* allocator_realloc(allocator allocator, void* ptr, size_t size);
void allocator_free(allocator allocator, void* ptr);

void* basic_alloc(void* _, size_t size);
void* basic_realloc(void* _, void* ptr, size_t size);
void basic_free(void* _, void* ptr);
allocator basic_allocator(void);
