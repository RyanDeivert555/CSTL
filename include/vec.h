#pragma once
#include <string.h> // IWYU pragma: keep
#include "allocator.h" // IWYU pragma: keep

#define VEC_DEFINE(T) \
    typedef struct vec_##T { \
        T* ptr; \
        size_t length; \
        size_t capacity; \
    } vec_##T; \
    \
    vec_##T vec_##T##_new(void); \
    void vec_##T##_free(allocator allocator, vec_##T* vec); \
    void vec_##T##_grow(allocator allocator, vec_##T* vec); \
    void vec_##T##_push(allocator allocator, vec_##T* vec, T elem); \
    T vec_##T##_pop(vec_##T* vec); \
    void vec_##T##_insert(allocator allocator, vec_##T* vec, T elem, size_t index); \
    T vec_##T##_remove(vec_##T* vec, size_t index); \
    void vec_##T##_reserve(allocator allocator, vec_##T* vec, size_t extra_spaces); \
    void vec_##T##_append(allocator allocator, vec_##T* dest, vec_##T src); \

#define VEC_IMPL(T) \
    vec_##T vec_##T##_new(void) { \
        vec_##T result = {0}; \
        \
        return result; \
    } \
    \
    void vec_##T##_free(allocator allocator, vec_##T* vec) { \
        allocator_free(T, allocator, vec->ptr, vec->capacity); \
    } \
    \
    void vec_##T##_grow(allocator allocator, vec_##T* vec) { \
        size_t new_capacity = (vec->capacity == 0) ? 2 : 2 * vec->capacity; \
        allocator_realloc(T, allocator, &vec->ptr, vec->capacity, new_capacity); \
        vec->capacity = new_capacity; \
    } \
    \
    void vec_##T##_push(allocator allocator, vec_##T* vec, T elem) { \
        if (vec->length == vec->capacity) { \
            vec_##T##_grow(allocator, vec); \
        } \
        vec->ptr[vec->length] = elem; \
        vec->length++; \
    } \
    \
    T vec_##T##_pop(vec_##T* vec) { \
        assert(vec->length > 0); \
        size_t last_index = vec->length - 1; \
        T value = vec->ptr[last_index]; \
        vec->length--; \
        \
        return value; \
    } \
    \
    void vec_##T##_insert(allocator allocator, vec_##T* vec, T elem, size_t index) { \
        assert(index <= vec->length); \
        if (vec->length == vec->capacity) { \
            vec_##T##_grow(allocator, vec); \
        } \
        memmove(&vec->ptr[index + 1], &vec->ptr[index], (vec->length - index) * sizeof(T)); \
        vec->ptr[index] = elem; \
        vec->length++; \
    } \
    \
    T vec_##T##_remove(vec_##T* vec, size_t index) { \
        assert(index < vec->length); \
        vec->length--; \
        T res = vec->ptr[index]; \
        memmove(&vec->ptr[index], &vec->ptr[index + 1], (vec->length - index) * sizeof(T)); \
        \
        return res; \
    } \
    \
    void vec_##T##_reserve(allocator allocator, vec_##T* vec, size_t extra_spaces) { \
        allocator_realloc(T, allocator, &vec->ptr, vec->capacity, (vec->capacity + extra_spaces)); \
        vec->capacity += extra_spaces; \
    } \
    \
    void vec_##T##_append(allocator allocator, vec_##T* dest, vec_##T src) { \
        size_t count = src.length; \
        vec_##T##_reserve(allocator, dest, count); \
        memcpy(&dest->ptr[dest->length], src.ptr, count * sizeof(T)); \
        dest->length += count; \
    }

