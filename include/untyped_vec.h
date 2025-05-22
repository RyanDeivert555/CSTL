#pragma once
#include "common.h"
#include "allocator.h"

typedef struct UntypedVec {
    void* buffer;
    i64 length;
    i64 capacity;
} UntypedVec;

UntypedVec UntypedVecNew(void);
void UntypedVecFree(UntypedVec* vec, Allocator allocator, i64 type_size, i64 align);
void UntypedVecPush(UntypedVec* vec, Allocator allocator, i64 type_size, i64 align, const void* const item);
void* UntypedVecPop(UntypedVec* vec, i64 type_size);
void UntypedVecInsert(UntypedVec* vec, Allocator allocator, i64 index, i64 type_size, i64 align, const void* const item);
void UntypedVecRemove(UntypedVec* vec, i64 index, i64 type_size);
void UntypedVecReserve(UntypedVec* vec, Allocator allocator, i64 type_size, i64 align, i64 new_capacity);
void* UntypedVecGet(UntypedVec* vec, i64 index, i64 type_size);
void UntypedVecSet(UntypedVec* vec, i64 index, i64 type_size, const void* const item);

