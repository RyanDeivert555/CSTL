#include "CSTL/untyped_vec.h"
#include "CSTL/allocator.h"
#include "CSTL/common.h"
#include <string.h>

void untyped_vec_free(untyped_vec* vec, i64 item_size, i64 item_align) {
    cstl_raw_free((u8*)vec->buffer, item_size, vec->capacity, item_align);
}

void untyped_vec_push(untyped_vec* vec, i64 item_size, i64 item_align, const void* item) {
    if (vec->length == vec->capacity) {
        const i64 new_capacity = (vec->capacity == 0) ? 1 : vec->capacity * 2;
        untyped_vec_reserve(vec, item_size, item_align, new_capacity);
    }
    memcpy((u8*)vec->buffer + vec->length * item_size, item, item_size);
    vec->length++;
}

void* untyped_vec_pop(untyped_vec* vec, i64 item_size) {
    cstl_assert(vec->length > 0);
    const i64 last_index = vec->length - 1;
    void* const result = (u8*)vec->buffer + last_index * item_size;
    vec->length--;

    return result;
}

void untyped_vec_insert(untyped_vec* vec, i64 index, i64 item_size, i64 item_align, const void* item) {
    cstl_assert(index >= 0 && index <= vec->length);
    if (vec->length == vec->capacity) {
        const i64 new_capacity = (vec->capacity == 0) ? 1 : vec->capacity * 2;
        untyped_vec_reserve(vec, item_size, item_align, new_capacity);
    }
    memmove((u8*)vec->buffer + (index + 1) * item_size, (u8*)vec->buffer + index * item_size,
            (vec->length - index) * item_size);
    memcpy((u8*)vec->buffer + index * item_size, item, item_size);
    vec->length++;
}

void untyped_vec_remove(untyped_vec* vec, i64 index, i64 item_size) {
    cstl_assert(index >= 0 && index < vec->length);
    vec->length--;
    memmove((u8*)vec->buffer + index * item_size, (u8*)vec->buffer + (index + 1) * item_size,
            (vec->length - index) * item_size);
}

void untyped_vec_reserve(untyped_vec* vec, i64 item_size, i64 item_align, i64 new_capacity) {
    cstl_assert(new_capacity >= 0);
    void* const new_buffer = cstl_raw_alloc(item_size, new_capacity, item_align);
    if (new_capacity < vec->length) {
        vec->length = new_capacity;
    }
    if (vec->length != 0) {
        memcpy(new_buffer, vec->buffer, item_size * vec->length);
    }
    cstl_raw_free((u8*)vec->buffer, item_size, vec->capacity, item_align);
    vec->buffer = new_buffer;
    vec->capacity = new_capacity;
}

void* untyped_vec_get(untyped_vec* vec, i64 index, i64 item_size) {
    cstl_assert(index >= 0 && index < vec->length);

    return (u8*)vec->buffer + index * item_size;
}

void untyped_vec_set(untyped_vec* vec, i64 index, i64 item_size, const void* item) {
    cstl_assert(index >= 0 && index < vec->length);

    memcpy((u8*)vec->buffer + index * item_size, item, item_size);
}
