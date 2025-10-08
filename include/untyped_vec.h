#pragma once
#include "common.h"
#include "allocator.h"

typedef struct UntypedVec {
    void* buffer;
    I64 length;
    I64 capacity;
} UntypedVec;

void UntypedVecFree(UntypedVec* vec, Allocator allocator, I64 item_size, I64 item_align);
void UntypedVecPush(UntypedVec* vec, Allocator allocator, I64 item_size, I64 item_align, const void* const item);
void* UntypedVecPop(UntypedVec* vec, I64 item_size);
void UntypedVecInsert(UntypedVec* vec, Allocator allocator, I64 index, I64 item_size, I64 item_align, const void* const item);
void UntypedVecRemove(UntypedVec* vec, I64 index, I64 item_size);
void UntypedVecReserve(UntypedVec* vec, Allocator allocator, I64 item_size, I64 item_align, I64 new_capacity);
void* UntypedVecGet(UntypedVec* vec, I64 index, I64 item_size);
void UntypedVecSet(UntypedVec* vec, I64 index, I64 item_size, const void* const item);

