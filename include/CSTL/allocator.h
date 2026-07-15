#pragma once
#include "common.h"
#include <stdalign.h>

typedef u8* (*alloc_func)(void* ctx, i64 size, i64 count, i64 align);
typedef void (*free_func)(void* ctx, u8* ptr, i64 size, i64 count, i64 align);

typedef struct allocator_vtable {
    alloc_func alloc;
    free_func free;
} allocator_vtable;

typedef struct allocator {
    void* ctx;
    const allocator_vtable* vtable;
} allocator;

u8* cstl_raw_alloc(allocator a, i64 size, i64 count, i64 align);
void cstl_raw_free(allocator a, u8* ptr, i64 size, i64 count, i64 align);

allocator std_allocator(void);

#define cstl_alloc(T, allocator, count) (T*)cstl_raw_alloc(allocator, sizeof(T), (count), alignof(T))
#define cstl_free(T, allocator, ptr, count) cstl_raw_free(allocator, (u8*)(ptr), sizeof(T), (count), alignof(T))
