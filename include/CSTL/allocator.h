#pragma once
#include "common.h"
#include <stdalign.h>

typedef struct allocator_v_table {
    u8* (*alloc)(void* ctx, i64 size, i64 count, i64 align);
    void (*free)(void* ctx, u8* ptr, i64 size, i64 count, i64 align);
} allocator_v_table;

typedef struct allocator {
    void* ctx;
    const allocator_v_table* vtable;
} allocator;

u8* allocator_raw_alloc(allocator allocator, i64 size, i64 count, i64 align);
void allocator_raw_free(allocator allocator, u8* ptr, i64 size, i64 count, i64 align);

#define allocator_alloc(T, allocator, count) (T*)allocator_raw_alloc(allocator, sizeof(T), (count), alignof(T))
#define allocator_free(T, allocator, ptr, count)                                                                       \
    allocator_raw_free(allocator, (u8*)(ptr), sizeof(T), (count), alignof(T))

allocator std_allocator(void);
