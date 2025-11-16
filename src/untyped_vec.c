#include "CSTL/untyped_vec.h"
#include "CSTL/allocator.h"
#include "CSTL/common.h"
#include <string.h>

void UntypedVecFree(UntypedVec* vec, Allocator allocator, I64 item_size, I64 item_align) {
    AllocatorRawFree(allocator, (U8*)vec->buffer, item_size, vec->capacity, item_align);
}

void UntypedVecPush(UntypedVec* vec, Allocator allocator, I64 item_size, I64 item_align, const void* const item) {
    if (vec->length == vec->capacity) {
        const I64 new_capacity = (vec->capacity == 0) ? 1 : vec->capacity * 2;
        UntypedVecReserve(vec, allocator, item_size, item_align, new_capacity);
    }
    memcpy((U8*)vec->buffer + vec->length * item_size, item, item_size);
    vec->length++;
}

void* UntypedVecPop(UntypedVec* vec, I64 item_size) {
    Assert(vec->length > 0);
    const I64 last_index = vec->length - 1;
    void* result = (U8*)vec->buffer + last_index * item_size;
    vec->length--;

    return result;
}

void UntypedVecInsert(UntypedVec* vec, Allocator allocator, I64 index, I64 item_size, I64 item_align,
                      const void* const item) {
    Assert(index >= 0 && index <= vec->length);
    if (vec->length == vec->capacity) {
        const I64 new_capacity = (vec->capacity == 0) ? 1 : vec->capacity * 2;
        UntypedVecReserve(vec, allocator, item_size, item_align, new_capacity);
    }
    memmove((U8*)vec->buffer + (index + 1) * item_size, (U8*)vec->buffer + index * item_size,
            (vec->length - index) * item_size);
    memcpy((U8*)vec->buffer + index * item_size, item, item_size);
    vec->length++;
}

void UntypedVecRemove(UntypedVec* vec, I64 index, I64 item_size) {
    Assert(index >= 0 && index < vec->length);
    vec->length--;
    memmove((U8*)vec->buffer + index * item_size, (U8*)vec->buffer + (index + 1) * item_size,
            (vec->length - index) * item_size);
}

void UntypedVecReserve(UntypedVec* vec, Allocator allocator, I64 item_size, I64 item_align, I64 new_capacity) {
    void* new_buffer = AllocatorRawAlloc(allocator, item_size, new_capacity, item_align);
    if (vec->length != 0) {
        memcpy(new_buffer, vec->buffer, item_size * vec->length);
    }
    AllocatorRawFree(allocator, (U8*)vec->buffer, item_size, vec->capacity, item_size);
    vec->buffer = new_buffer;
    vec->capacity = new_capacity;
}

void* UntypedVecGet(UntypedVec* vec, I64 index, I64 item_size) {
    Assert(index >= 0 && index < vec->length);

    return (U8*)vec->buffer + index * item_size;
}

void UntypedVecSet(UntypedVec* vec, I64 index, I64 item_size, const void* const item) {
    Assert(index >= 0 && index < vec->length);

    memcpy((U8*)vec->buffer + index * item_size, item, item_size);
}
