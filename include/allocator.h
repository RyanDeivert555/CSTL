#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef struct allocator_vtable {
    unsigned char* (*alloc)(void* ctx, size_t bytes, size_t align);
    bool (*realloc)(void* vtx, unsigned char** ptr, size_t current_size, size_t new_size, size_t align);
    void (*free)(void* ctx, unsigned char* ptr, size_t size, size_t align);
} allocator_vtable;

typedef struct allocator {
    void* ctx;
    const allocator_vtable* vtable;
} allocator;

unsigned char* allocator_raw_alloc(allocator allocator, size_t bytes, size_t align);
bool allocator_raw_realloc(allocator allocator, unsigned char** ptr, size_t current_size, size_t new_size, size_t align);
void allocator_raw_free(allocator allocator, unsigned char* ptr, size_t size, size_t align);

#define allocator_create(T, allocator) \
    (T*)allocator_raw_alloc(allocator, sizeof(T), _Alignof(T))
#define allocator_alloc(T, allocator, length) \
    (T*)allocator_raw_alloc(allocator, sizeof(T) * length, _Alignof(T))
#define allocator_realloc(T, allocator, ptr, current_length, new_length) \
    allocator_raw_realloc(allocator, (unsigned char**)ptr, sizeof(T) * current_length, sizeof(T) * new_length, _Alignof(T))
#define allocator_destroy(T, allocator, ptr) \
    allocator_raw_free(allocator, (unsigned char*)ptr, sizeof(T), _Alignof(T))
#define allocator_free(T, allocator, ptr, length) \
    allocator_raw_free(allocator, (unsigned char*)ptr, sizeof(T) * length, _Alignof(T))

allocator c_allocator(void);

