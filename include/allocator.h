#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef struct allocator_vtable {
    unsigned char* (*alloc)(void* ctx, size_t bytes);
    bool (*realloc)(void* vtx, unsigned char** ptr, size_t current_size, size_t new_size);
    void (*free)(void* ctx, unsigned char* ptr, size_t size);
} allocator_vtable;

typedef struct allocator {
    void* ctx;
    const allocator_vtable* vtable;
} allocator;

unsigned char* allocator_raw_alloc(allocator allocator, size_t bytes);
bool allocator_raw_realloc(allocator allocator, unsigned char** ptr, size_t current_size, size_t new_size);
void allocator_raw_free(allocator allocator, unsigned char* ptr, size_t size);

#define allocator_create(T, allocator) \
    (T*)allocator_raw_alloc(allocator, sizeof(T))
#define allocator_alloc(T, allocator, length) \
    (T*)allocator_raw_alloc(allocator, sizeof(T) * length)
#define allocator_realloc(T, allocator, ptr, current_length, new_length) \
    allocator_raw_realloc(allocator, (unsigned char**)ptr, sizeof(T) * current_length, sizeof(T) * new_length)
#define allocator_destroy(T, allocator, ptr) \
    allocator_raw_free(allocator, (unsigned char*)ptr, sizeof(T))
#define allocator_free(T, allocator, ptr, length) \
    allocator_raw_free(allocator, (unsigned char*)ptr, sizeof(T) * length)

allocator c_allocator(void);

// TODO: fba not portable, requires -fno-strict-aliasing
typedef struct fba {
    unsigned char* buffer;
    size_t length;
    size_t end_index;
} fba;

fba fba_new(unsigned char* buffer, size_t length);
allocator fba_as_allocator(fba* fba);

