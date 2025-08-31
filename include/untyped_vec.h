#pragma once
#include "common.h"
#include "allocator.h"

typedef struct UntypedVec {
    void* buffer;
    i64 length;
    i64 capacity;
} UntypedVec;

void UntypedVecFree(UntypedVec* vec, Allocator allocator, i64 item_size, i64 item_align);
void UntypedVecPush(UntypedVec* vec, Allocator allocator, i64 item_size, i64 item_align, const void* const item);
void* UntypedVecPop(UntypedVec* vec, i64 item_size);
void UntypedVecInsert(UntypedVec* vec, Allocator allocator, i64 index, i64 item_size, i64 item_align, const void* const item);
void UntypedVecRemove(UntypedVec* vec, i64 index, i64 item_size);
void UntypedVecReserve(UntypedVec* vec, Allocator allocator, i64 item_size, i64 item_align, i64 new_capacity);
void* UntypedVecGet(UntypedVec* vec, i64 index, i64 item_size);
void UntypedVecSet(UntypedVec* vec, i64 index, i64 item_size, const void* const item);

