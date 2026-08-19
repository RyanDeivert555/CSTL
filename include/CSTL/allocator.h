#pragma once
#include "common.h"
#include <stdalign.h>

typedef cstl_byte* (*alloc_func)(void* ctx, isize size, isize count, isize align);
typedef void (*free_func)(void* ctx, cstl_byte* ptr, isize size, isize count, isize align);

typedef struct allocator_vtable {
    alloc_func alloc;
    free_func free;
} allocator_vtable;

typedef struct allocator {
    void* ctx;
    const allocator_vtable* vtable;
} allocator;

bool cstl_is_pow2(isize n);
isize cstl_next_pow2(isize n);

cstl_byte* cstl_raw_alloc(allocator a, isize size, isize count, isize align);
void cstl_raw_free(allocator a, cstl_byte* ptr, isize size, isize count, isize align);

allocator std_allocator(void);

#define cstl_alloc(T, allocator, count) (T*)cstl_raw_alloc(allocator, sizeof(T), (count), alignof(T))
#define cstl_free(T, allocator, ptr, count) cstl_raw_free(allocator, (cstl_byte*)(ptr), sizeof(T), (count), alignof(T))
