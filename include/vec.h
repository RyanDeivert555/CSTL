#pragma once
#include "allocator.h" // IWYU pragma: keep
#include "common.h"    // IWYU pragma: keep
#include <string.h>    // IWYU pragma: keep

#define VEC_DEFINE(T)                                                                                                  \
    typedef struct Vec##T {                                                                                            \
        T* buffer;                                                                                                     \
        I64 length;                                                                                                    \
        I64 capacity;                                                                                                  \
    } Vec##T;                                                                                                          \
                                                                                                                       \
    void Vec##T##Free(Vec##T* vec, Allocator allocator);                                                               \
    void Vec##T##Push(Vec##T* vec, Allocator allocator, T elem);                                                       \
    T Vec##T##Pop(Vec##T* vec);                                                                                        \
    void Vec##T##Insert(Vec##T* vec, Allocator allocator, T elem, I64 index);                                          \
    T Vec##T##Remove(Vec##T* vec, I64 index);                                                                          \
    void Vec##T##Reserve(Vec##T* vec, Allocator allocator, I64 new_capacity);

#define VEC_IMPL(T)                                                                                                    \
    void Vec##T##Free(Vec##T* vec, Allocator allocator) {                                                              \
        AllocatorFree(T, allocator, vec->buffer, vec->capacity);                                                       \
    }                                                                                                                  \
                                                                                                                       \
    void Vec##T##Push(Vec##T* vec, Allocator allocator, T elem) {                                                      \
        if (vec->length == vec->capacity) {                                                                            \
            const I64 new_capacity = (vec->capacity == 0) ? 1 : vec->capacity * 2;                                     \
            Vec##T##Reserve(vec, allocator, new_capacity);                                                             \
        }                                                                                                              \
        vec->buffer[vec->length] = elem;                                                                               \
        vec->length++;                                                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    T Vec##T##Pop(Vec##T* vec) {                                                                                       \
        Assert(vec->length > 0);                                                                                       \
        const I64 last_index = vec->length - 1;                                                                        \
        const T value = vec->buffer[last_index];                                                                       \
        vec->length--;                                                                                                 \
                                                                                                                       \
        return value;                                                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    void Vec##T##Insert(Vec##T* vec, Allocator allocator, T elem, I64 index) {                                         \
        Assert(index >= 0 && index <= vec->length);                                                                    \
        if (vec->length == vec->capacity) {                                                                            \
            const I64 new_capacity = (vec->capacity == 0) ? 1 : vec->capacity * 2;                                     \
            Vec##T##Reserve(vec, allocator, new_capacity);                                                             \
        }                                                                                                              \
        memmove(&vec->buffer[index + 1], &vec->buffer[index], (vec->length - index) * sizeof(T));                      \
        vec->buffer[index] = elem;                                                                                     \
        vec->length++;                                                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    T Vec##T##Remove(Vec##T* vec, I64 index) {                                                                         \
        Assert(index >= 0 && index < vec->length);                                                                     \
        vec->length--;                                                                                                 \
        const T res = vec->buffer[index];                                                                              \
        memmove(&vec->buffer[index], &vec->buffer[index + 1], (vec->length - index) * sizeof(T));                      \
                                                                                                                       \
        return res;                                                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    void Vec##T##Reserve(Vec##T* vec, Allocator allocator, I64 new_capacity) {                                         \
        Assert(new_capacity >= 0);                                                                                     \
        T* new_buffer = AllocatorAlloc(T, allocator, new_capacity);                                                    \
        if (vec->length != 0) {                                                                                        \
            memcpy(new_buffer, vec->buffer, vec->length * sizeof(T));                                                  \
        }                                                                                                              \
        AllocatorFree(T, allocator, vec->buffer, vec->capacity);                                                       \
        vec->buffer = new_buffer;                                                                                      \
        vec->capacity = new_capacity;                                                                                  \
    }
