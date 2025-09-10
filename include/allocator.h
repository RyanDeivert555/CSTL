#pragma once
#include "common.h"
#include <stdalign.h>

typedef struct AllocatorVTable {
    u8* (*alloc)(void* ctx, i64 size, i64 count, i64 align);
    void (*free)(void* ctx, u8* ptr, i64 size, i64 count, i64 align);
} AllocatorVTable;

typedef struct Allocator {
    void* ctx;
    const AllocatorVTable* vtable;
} Allocator;

u8* AllocatorRawAlloc(Allocator allocator, i64 size, i64 count, i64 align);
void AllocatorRawFree(Allocator allocator, u8* ptr, i64 size, i64 count, i64 align);

#define AllocatorAlloc(T, allocator, count) \
    (T*)AllocatorRawAlloc(allocator, sizeof(T), (count), alignof(T))
#define AllocatorFree(T, allocator, ptr, count) \
    AllocatorRawFree(allocator, (u8*)(ptr), sizeof(T), (count), alignof(T))

Allocator StdAllocator(void);

