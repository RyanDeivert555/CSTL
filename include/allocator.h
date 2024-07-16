#pragma once
#include <stddef.h>

typedef struct mem_slice {
    void* ptr;
    size_t length;
} mem_slice;

typedef struct allocator_vtable {
    void* (*alloc)(void* ctx, size_t size);
    mem_slice (*alloc_slice)(void* ctx, size_t elem_size, size_t size);
    void* (*realloc)(void* vtx, void* ptr, size_t size);
    mem_slice (*realloc_slice)(void* ctx, mem_slice slice, size_t elem_size, size_t size);
    void (*free)(void* ctx, void* ptr);
    void (*free_slice)(void* ctx, mem_slice slice);
} allocator_vtable;

typedef struct allocator {
    void* ctx;
    allocator_vtable vtable;
} allocator;

void* allocator_alloc(allocator allocator, size_t size);
mem_slice allocator_alloc_slice(allocator allocator, size_t elem_size, size_t size);
void* allocator_realloc(allocator allocator, void* ptr, size_t size);
mem_slice allocator_realloc_slice(allocator allocator, mem_slice slice, size_t elem_size, size_t size);
void allocator_free(allocator allocator, void* ptr);
void allocator_free_slice(allocator allocator, mem_slice slice);

void* basic_alloc(void* _, size_t size);
mem_slice basic_alloc_slice(void* _, size_t elem_size, size_t size);
void* basic_realloc(void* _, void* ptr, size_t size);
mem_slice basic_realloc_slice(void* _, mem_slice slice, size_t elem_size, size_t size);
void basic_free(void* _, void* ptr);
void basic_free_slice(void* _, mem_slice slice);
allocator basic_allocator(void);
