#pragma once
#include "common.h"
#include "allocator.h"

typedef struct UntypedVec {
    void* buffer;
    i64 length;
    i64 capacity;
} UntypedVec;

UntypedVec UntypedVecNew(void);
void UntypedVecFree(UntypedVec* vec, Allocator allocator, TypeInfo type_info);
void UntypedVecPush(UntypedVec* vec, Allocator allocator, TypeInfo type_info, const void* const item);
void* UntypedVecPop(UntypedVec* vec, TypeInfo type_info);
void UntypedVecInsert(UntypedVec* vec, Allocator allocator, i64 index, TypeInfo type_info, const void* const item);
void UntypedVecRemove(UntypedVec* vec, i64 index, TypeInfo type_info);
void UntypedVecReserve(UntypedVec* vec, Allocator allocator, TypeInfo type_info, i64 new_capacity);
void* UntypedVecGet(UntypedVec* vec, i64 index, TypeInfo type_info);
void UntypedVecSet(UntypedVec* vec, i64 index, TypeInfo type_info, const void* const item);

