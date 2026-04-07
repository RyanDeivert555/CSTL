#pragma once
#include "common.h"
#include <stdalign.h>

typedef u8* (*alloc_func)(void* ctx, i64 size, i64 count, i64 align);
typedef void (*free_func)(void* ctx, u8* ptr, i64 size, i64 count, i64 align);

void set_global_allocator(void* ctx, alloc_func alloc, free_func free);
u8* cstl_raw_alloc(i64 size, i64 count, i64 align);
void cstl_raw_free(u8* ptr, i64 size, i64 count, i64 align);

u8* std_alloc(void* ctx, i64 size, i64 count, i64 align);
void std_free(void* ctx, u8* ptr, i64 size, i64 count, i64 align);

#define cstl_alloc(T, count) (T*)cstl_raw_alloc(sizeof(T), (count), alignof(T))
#define cstl_free(T, ptr, count) cstl_raw_free((u8*)(ptr), sizeof(T), (count), alignof(T))
