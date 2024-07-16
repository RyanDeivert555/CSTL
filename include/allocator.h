#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef struct allocator_vtable {
    char* (*alloc)(void* ctx, size_t bytes);
    bool (*realloc)(void* vtx, char** ptr, size_t current_size, size_t new_size);
    void (*free)(void* ctx, char* ptr, size_t size);
} allocator_vtable;

typedef struct allocator {
    void* ctx;
    const allocator_vtable* vtable;
} allocator;

char* allocator_raw_alloc(allocator allocator, size_t bytes);
bool allocator_raw_realloc(allocator allocator, char** ptr, size_t current_size, size_t new_size);
void allocator_raw_free(allocator allocator, char* ptr, size_t size);

allocator c_allocator(void);
