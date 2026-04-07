#pragma once
#include "allocator.h" // IWYU pragma: keep
#include "common.h"    // IWYU pragma: keep
#include <string.h>    // IWYU pragma: keep

#define VEC_DEFINE(T)                                                                                                  \
    typedef struct vec_##T {                                                                                           \
        T* buffer;                                                                                                     \
        i64 length;                                                                                                    \
        i64 capacity;                                                                                                  \
    } vec_##T;                                                                                                         \
                                                                                                                       \
    void vec_##T##_free(vec_##T* vec);                                                                                 \
    void vec_##T##_push(vec_##T* vec, T elem);                                                                         \
    T vec_##T##_pop(vec_##T* vec);                                                                                     \
    void vec_##T##_insert(vec_##T* vec, T elem, i64 index);                                                            \
    T vec_##T##_remove(vec_##T* vec, i64 index);                                                                       \
    void vec_##T##_reserve(vec_##T* vec, i64 new_capacity);

#define VEC_IMPL(T)                                                                                                    \
    void vec_##T##_free(vec_##T* vec) {                                                                                \
        cstl_free(T, vec->buffer, vec->capacity);                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    void vec_##T##_push(vec_##T* vec, T elem) {                                                                        \
        if (vec->length == vec->capacity) {                                                                            \
            const i64 new_capacity = (vec->capacity == 0) ? 1 : vec->capacity * 2;                                     \
            vec_##T##_reserve(vec, new_capacity);                                                                      \
        }                                                                                                              \
        vec->buffer[vec->length] = elem;                                                                               \
        vec->length++;                                                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    T vec_##T##_pop(vec_##T* vec) {                                                                                    \
        cstl_assert(vec->length > 0);                                                                                  \
        const i64 last_index = vec->length - 1;                                                                        \
        const T value = vec->buffer[last_index];                                                                       \
        vec->length--;                                                                                                 \
                                                                                                                       \
        return value;                                                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    void vec_##T##_insert(vec_##T* vec, T elem, i64 index) {                                                           \
        cstl_assert(index >= 0 && index <= vec->length);                                                               \
        if (vec->length == vec->capacity) {                                                                            \
            const i64 new_capacity = (vec->capacity == 0) ? 1 : vec->capacity * 2;                                     \
            vec_##T##_reserve(vec, new_capacity);                                                                      \
        }                                                                                                              \
        memmove(&vec->buffer[index + 1], &vec->buffer[index], (vec->length - index) * sizeof(T));                      \
        vec->buffer[index] = elem;                                                                                     \
        vec->length++;                                                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    T vec_##T##_remove(vec_##T* vec, i64 index) {                                                                      \
        cstl_assert(index >= 0 && index < vec->length);                                                                \
        vec->length--;                                                                                                 \
        const T res = vec->buffer[index];                                                                              \
        memmove(&vec->buffer[index], &vec->buffer[index + 1], (vec->length - index) * sizeof(T));                      \
                                                                                                                       \
        return res;                                                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    void vec_##T##_reserve(vec_##T* vec, i64 new_capacity) {                                                           \
        cstl_assert(new_capacity >= 0);                                                                                \
        T* const new_buffer = cstl_alloc(T, new_capacity);                                                             \
        if (vec->length != 0) {                                                                                        \
            memcpy(new_buffer, vec->buffer, vec->length * sizeof(T));                                                  \
        }                                                                                                              \
        cstl_free(T, vec->buffer, vec->capacity);                                                                      \
        vec->buffer = new_buffer;                                                                                      \
        vec->capacity = new_capacity;                                                                                  \
    }
