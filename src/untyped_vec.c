#include "untyped_vec.h"
#include "allocator.h"
#include "common.h"
#include <string.h>

UntypedVec UntypedVecNew(void) {
    const UntypedVec result = {0};

    return result;
}

void UntypedVecFree(UntypedVec* vec, Allocator allocator, TypeInfo type_info) {
    AllocatorRawFree(allocator, (u8*)vec->buffer, type_info.size, vec->capacity, type_info.align);
}

void UntypedVecPush(UntypedVec* vec, Allocator allocator, TypeInfo type_info, const void* const item) {
    if (vec->length == vec->capacity) {
        const i64 new_capacity = (vec->capacity == 0) ? 1 : vec->capacity * 2;
        UntypedVecReserve(vec, allocator, type_info, new_capacity);
    }
    memmove((u8*)vec->buffer + vec->length * type_info.size, item, type_info.size);
    vec->length++;
}

void* UntypedVecPop(UntypedVec* vec, TypeInfo type_info) {
    Assert(vec->length > 0);
    const i64 last_index = vec->length - 1;
    void* result = (u8*)vec->buffer + last_index * type_info.size;
    vec->length--;

    return result;
}

void UntypedVecInsert(UntypedVec* vec, Allocator allocator, i64 index, TypeInfo type_info, const void* const item) {
    Assert(index >= 0 && index <= vec->length);
    if (vec->length == vec->capacity) {
        const i64 new_capacity = (vec->capacity == 0) ? 1 : vec->capacity * 2;
        UntypedVecReserve(vec, allocator, type_info, new_capacity);
    }
    memmove((u8*)vec->buffer + (index + 1) * type_info.size, (u8*)vec->buffer + index * type_info.size, (vec->length - index) * type_info.size);
    memmove((u8*)vec->buffer + index * type_info.size, item, type_info.size);
    vec->length++;
}

void UntypedVecRemove(UntypedVec* vec, i64 index, TypeInfo type_info) {
    Assert(index >= 0 && index < vec->length);
    vec->length--;
    memmove((u8*)vec->buffer + index * type_info.size, (u8*)vec->buffer + (index + 1) * type_info.size, (vec->length - index) * type_info.size);
}

void UntypedVecReserve(UntypedVec* vec, Allocator allocator, TypeInfo type_info, i64 new_capacity) {
    void* new_buffer = AllocatorRawAlloc(allocator, type_info.size, new_capacity, type_info.align);
    if (vec->length != 0) {
        memcpy(new_buffer, vec->buffer, type_info.size * vec->length);
    }
    AllocatorRawFree(allocator, (u8*)vec->buffer, type_info.size, vec->capacity, type_info.size);
    vec->buffer = new_buffer;
    vec->capacity = new_capacity;
}

void* UntypedVecGet(UntypedVec* vec, i64 index, TypeInfo type_info) {
    Assert(index >= 0 && index < vec->length);

    return (u8*)vec->buffer + index * type_info.size;
}

void UntypedVecSet(UntypedVec* vec, i64 index, TypeInfo type_info, const void* const item) {
    Assert(index >= 0 && index < vec->length);

    memmove((u8*)vec->buffer + index * type_info.size, item, type_info.size);
}

