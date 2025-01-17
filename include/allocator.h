#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct allocator_vtable {
    uint8_t* (*alloc)(void* ctx, int64_t bytes, int64_t align);
    void (*free)(void* ctx, uint8_t* ptr, int64_t size, int64_t align);
} allocator_vtable;

typedef struct allocator {
    void* ctx;
    const allocator_vtable* vtable;
} allocator;

uint8_t* allocator_raw_alloc(allocator allocator, int64_t bytes, int64_t align);
void allocator_raw_free(allocator allocator, uint8_t* ptr, int64_t size, int64_t align);

#define allocator_create(T, allocator) \
    (T*)allocator_raw_alloc(allocator, sizeof(T), _Alignof(T))
#define allocator_alloc(T, allocator, length) \
    (T*)allocator_raw_alloc(allocator, sizeof(T) * length, _Alignof(T))
#define allocator_destroy(T, allocator, ptr) \
    allocator_raw_free(allocator, (uint8_t*)ptr, sizeof(T), _Alignof(T))
#define allocator_free(T, allocator, ptr, length) \
    allocator_raw_free(allocator, (uint8_t*)ptr, sizeof(T) * length, _Alignof(T))

allocator c_allocator(void);

