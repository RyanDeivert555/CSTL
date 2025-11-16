#pragma once
#include "common.h"
#include <stdalign.h>

typedef struct AllocatorVTable {
    U8* (*alloc)(void* ctx, I64 size, I64 count, I64 align);
    void (*free)(void* ctx, U8* ptr, I64 size, I64 count, I64 align);
} AllocatorVTable;

typedef struct Allocator {
    void* ctx;
    const AllocatorVTable* vtable;
} Allocator;

U8* AllocatorRawAlloc(Allocator allocator, I64 size, I64 count, I64 align);
void AllocatorRawFree(Allocator allocator, U8* ptr, I64 size, I64 count, I64 align);

#define AllocatorAlloc(T, allocator, count) (T*)AllocatorRawAlloc(allocator, sizeof(T), (count), alignof(T))
#define AllocatorFree(T, allocator, ptr, count) AllocatorRawFree(allocator, (U8*)(ptr), sizeof(T), (count), alignof(T))

Allocator StdAllocator(void);
