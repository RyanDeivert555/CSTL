#pragma once
#include "allocator.h" // IWYU pragma: keep
#include "common.h"    // IWYU pragma: keep
#include <string.h>    // IWYU pragma: keep

#define cstl_vec(T) vec_##T
#define cstl_vec_create(T, alloc) vec_##T##_create(alloc)
#define cstl_vec_free(T, v) vec_##T##_free(v)
#define cstl_vec_push(T, v, elem) vec_##T##_push(v, elem)
#define cstl_vec_pop(T, v) vec_##T##_pop(v)
#define cstl_vec_insert(T, v, elem, index) vec_##T##_insert(v, elem, index)
#define cstl_vec_ordered_remove(T, v, index) vec_##T##_ordered_remove(v, index)
#define cstl_vec_swap_remove(T, v, index) vec_##T##_swap_remove(v, index)
#define cstl_vec_reserve(T, v, new_capacity) vec_##T##_reserve(v, new_capacity)

#define VEC_DEFINE(T)                                                                                                  \
    typedef struct vec_##T {                                                                                           \
        T* buffer;                                                                                                     \
        i64 length;                                                                                                    \
        i64 capacity;                                                                                                  \
        allocator alloc;                                                                                               \
    } vec_##T;                                                                                                         \
                                                                                                                       \
    vec_##T vec_##T##_create(allocator a);                                                                             \
    void vec_##T##_free(vec_##T* vec);                                                                                 \
    void vec_##T##_push(vec_##T* vec, T elem);                                                                         \
    T vec_##T##_pop(vec_##T* vec);                                                                                     \
    void vec_##T##_insert(vec_##T* vec, T elem, i64 index);                                                            \
    T vec_##T##_ordered_remove(vec_##T* vec, i64 index);                                                               \
    T vec_##T##_swap_remove(vec_##T* vec, i64 index);                                                                  \
    void vec_##T##_reserve(vec_##T* vec, i64 new_capacity);

#define VEC_IMPL(T)                                                                                                    \
    vec_##T vec_##T##_create(allocator a) {                                                                            \
        vec_##T result = {0};                                                                                          \
        result.alloc = a;                                                                                              \
                                                                                                                       \
        return result;                                                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    void vec_##T##_free(vec_##T* vec) {                                                                                \
        cstl_free(T, vec->alloc, vec->buffer, vec->capacity);                                                          \
        vec->buffer = NULL;                                                                                            \
        vec->length = 0;                                                                                               \
        vec->capacity = 0;                                                                                             \
    }                                                                                                                  \
                                                                                                                       \
    void vec_##T##_push(vec_##T* vec, T elem) {                                                                        \
        if (vec->length == vec->capacity) {                                                                            \
            const i64 new_capacity = (vec->capacity == 0) ? CSTL_INITIAL_CAPACITY : vec->capacity * 2;                 \
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
            const i64 new_capacity = (vec->capacity == 0) ? CSTL_INITIAL_CAPACITY : vec->capacity * 2;                 \
            vec_##T##_reserve(vec, new_capacity);                                                                      \
        }                                                                                                              \
        memmove(&vec->buffer[index + 1], &vec->buffer[index], (vec->length - index) * sizeof(T));                      \
        vec->buffer[index] = elem;                                                                                     \
        vec->length++;                                                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    T vec_##T##_ordered_remove(vec_##T* vec, i64 index) {                                                              \
        cstl_assert(index >= 0 && index < vec->length);                                                                \
        vec->length--;                                                                                                 \
        const T res = vec->buffer[index];                                                                              \
        memmove(&vec->buffer[index], &vec->buffer[index + 1], (vec->length - index) * sizeof(T));                      \
                                                                                                                       \
        return res;                                                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    T vec_##T##_swap_remove(vec_##T* vec, i64 index) {                                                                 \
        cstl_assert(index >= 0 && index < vec->length);                                                                \
        const T res = vec->buffer[index];                                                                              \
        vec->buffer[index] = vec->buffer[vec->length - 1];                                                             \
        vec->length--;                                                                                                 \
                                                                                                                       \
        return res;                                                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    void vec_##T##_reserve(vec_##T* vec, i64 new_capacity) {                                                           \
        cstl_assert(new_capacity >= vec->length);                                                                      \
        if (new_capacity <= vec->capacity) {                                                                           \
            return;                                                                                                    \
        }                                                                                                              \
                                                                                                                       \
        T* const new_buffer = cstl_alloc(T, vec->alloc, new_capacity);                                                 \
        if (vec->length != 0) {                                                                                        \
            memcpy(new_buffer, vec->buffer, vec->length * sizeof(T));                                                  \
        }                                                                                                              \
                                                                                                                       \
        cstl_free(T, vec->alloc, vec->buffer, vec->capacity);                                                          \
        vec->buffer = new_buffer;                                                                                      \
        vec->capacity = new_capacity;                                                                                  \
    }
