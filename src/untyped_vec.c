#include "untyped_vec.h"
#include "allocator.h"
#include "common.h"
#include <string.h>

UntypedVec UntypedVecNew(void) {
    const UntypedVec result = {0};

    return result;
}

void UntypedVecFree(UntypedVec* vec, Allocator allocator, i64 type_size, i64 align) {
    AllocatorRawFree(allocator, (u8*)vec->buffer, type_size, vec->capacity, align);
}

void UntypedVecPush(UntypedVec* vec, Allocator allocator, i64 type_size, i64 align, const void* const item) {
    if (vec->length == vec->capacity) {
        const i64 new_capacity = (vec->capacity == 0) ? 1 : vec->capacity * 2;
        UntypedVecReserve(vec, allocator, type_size, align, new_capacity);
    }
    memmove((u8*)vec->buffer + vec->length * type_size, item, type_size);
    vec->length++;
}

void* UntypedVecPop(UntypedVec* vec, i64 type_size) {
    Assert(vec->length > 0);
    const i64 last_index = vec->length - 1;
    void* result = (u8*)vec->buffer + last_index * type_size;
    vec->length--;

    return result;
}

void UntypedVecInsert(UntypedVec* vec, Allocator allocator, i64 index, i64 type_size, i64 align, const void* const item) {
    Assert(index >= 0 && index <= vec->length);
    if (vec->length == vec->capacity) {
        const i64 new_capacity = (vec->capacity == 0) ? 1 : vec->capacity * 2;
        UntypedVecReserve(vec, allocator, type_size, align, new_capacity);
    }
    memmove((u8*)vec->buffer + (index + 1) * type_size, (u8*)vec->buffer + index * type_size, (vec->length - index) * type_size);
    memmove((u8*)vec->buffer + index * type_size, item, type_size);
    vec->length++;
}

void UntypedVecRemove(UntypedVec* vec, i64 index, i64 type_size) {
    Assert(index >= 0 && index < vec->length);
    vec->length--;
    memmove((u8*)vec->buffer + index * type_size, (u8*)vec->buffer + (index + 1) * type_size, (vec->length - index) * type_size);
}

void UntypedVecReserve(UntypedVec* vec, Allocator allocator, i64 type_size, i64 align, i64 new_capacity) {
    void* new_buffer = AllocatorRawAlloc(allocator, type_size, new_capacity, align);
    if (vec->length != 0) {
        memcpy(new_buffer, vec->buffer, type_size * vec->length);
    }
    AllocatorRawFree(allocator, (u8*)vec->buffer, type_size, vec->capacity, align);
    vec->buffer = new_buffer;
    vec->capacity = new_capacity;
}

void* UntypedVecGet(UntypedVec* vec, i64 index, i64 type_size) {
    Assert(index >= 0 && index < vec->length);

    return (u8*)vec->buffer + index * type_size;
}

void UntypedVecSet(UntypedVec* vec, i64 index, i64 type_size, const void* const item) {
    Assert(index >= 0 && index < vec->length);

    memmove((u8*)vec->buffer + index * type_size, item, type_size);
}

