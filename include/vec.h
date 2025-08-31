#pragma once
#include "allocator.h" // IWYU pragma: keep
#include "common.h" // IWYU pragma: keep
#include <string.h> // IWYU pragma: keep

#define VEC_DEFINE(T) \
    typedef struct Vec_##T { \
        T* buffer; \
        i64 length; \
        i64 capacity; \
    } Vec_##T; \
    \
    void Vec_##T##_Free(Vec_##T* vec, Allocator allocator); \
    void Vec_##T##_Push(Vec_##T* vec, Allocator allocator, T elem); \
    T Vec_##T##_Pop(Vec_##T* vec); \
    void Vec_##T##_Insert(Vec_##T* vec, Allocator allocator, T elem, i64 index); \
    T Vec_##T##_Remove(Vec_##T* vec, i64 index); \
    void Vec_##T##_Reserve(Vec_##T* vec, Allocator allocator, i64 new_capacity); \

#define VEC_IMPL(T) \
    void Vec_##T##_Free(Vec_##T* vec, Allocator allocator) { \
        AllocatorFree(T, allocator, vec->buffer, vec->capacity); \
    } \
    \
    void Vec_##T##_Push(Vec_##T* vec, Allocator allocator, T elem) { \
        if (vec->length == vec->capacity) { \
            const i64 new_capacity = (vec->capacity == 0) ? 1 : vec->capacity * 2; \
            Vec_##T##_Reserve(vec, allocator, new_capacity); \
        } \
        vec->buffer[vec->length] = elem; \
        vec->length++; \
    } \
    \
    T Vec_##T##_Pop(Vec_##T* vec) { \
        Assert(vec->length > 0); \
        const i64 last_index = vec->length - 1; \
        const T value = vec->buffer[last_index]; \
        vec->length--; \
        \
        return value; \
    } \
    \
    void Vec_##T##_Insert(Vec_##T* vec, Allocator allocator, T elem, i64 index) { \
        Assert(index >= 0 && index <= vec->length); \
        if (vec->length == vec->capacity) { \
            const i64 new_capacity = (vec->capacity == 0) ? 1 : vec->capacity * 2; \
            Vec_##T##_Reserve(vec, allocator, new_capacity); \
        } \
        memmove(&vec->buffer[index + 1], &vec->buffer[index], (vec->length - index) * sizeof(T)); \
        vec->buffer[index] = elem; \
        vec->length++; \
    } \
    \
    T Vec_##T##_Remove(Vec_##T* vec, i64 index) { \
        Assert(index >= 0 && index < vec->length); \
        vec->length--; \
        const T res = vec->buffer[index]; \
        memmove(&vec->buffer[index], &vec->buffer[index + 1], (vec->length - index) * sizeof(T)); \
        \
        return res; \
    } \
    \
    void Vec_##T##_Reserve(Vec_##T* vec, Allocator allocator, i64 new_capacity) { \
        Assert(new_capacity >= 0); \
        T* new_buffer = AllocatorAlloc(T, allocator, new_capacity); \
        if (vec->length != 0) { \
            memcpy(new_buffer, vec->buffer, vec->length * sizeof(T)); \
        } \
        AllocatorFree(T, allocator, vec->buffer, vec->capacity); \
        vec->buffer = new_buffer; \
        vec->capacity = new_capacity; \
    } \

