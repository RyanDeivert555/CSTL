#pragma once
#include "allocator.h" // IWYU pragma: keep
#include "common.h"    // IWYU pragma: keep

#define QUEUE_DEFINE(T)                                                                                                \
    typedef struct queue_##T {                                                                                         \
        T* buffer;                                                                                                     \
        i64 length;                                                                                                    \
        i64 capacity;                                                                                                  \
        i64 front;                                                                                                     \
        i64 back;                                                                                                      \
    } queue_##T;                                                                                                       \
                                                                                                                       \
    void queue_##T##_free(queue_##T* q);                                                                               \
    void queue_##T##_push(queue_##T* q, T elem);                                                                       \
    T queue_##T##_pop(queue_##T* q);                                                                                   \
    bool queue_##T##_try_pop(queue_##T* q, T* out);                                                                    \
    void queue_##T##_reserve(queue_##T* q, i64 new_capacity);

#define QUEUE_IMPL(T)                                                                                                  \
    void queue_##T##_free(queue_##T* q) {                                                                              \
        cstl_free(T, q->buffer, q->capacity);                                                                          \
    }                                                                                                                  \
                                                                                                                       \
    void queue_##T##_push(queue_##T* q, T elem) {                                                                      \
        if (q->length == q->capacity) {                                                                                \
            const i64 new_capacity = q->capacity == 0 ? 1 : q->capacity * 2;                                           \
            queue_##T##_reserve(q, new_capacity);                                                                      \
        }                                                                                                              \
                                                                                                                       \
        if (q->length == 0) {                                                                                          \
            q->front = 0;                                                                                              \
            q->back = 0;                                                                                               \
        } else {                                                                                                       \
            q->back = (q->back + 1) % q->capacity;                                                                     \
        }                                                                                                              \
                                                                                                                       \
        q->buffer[q->back] = elem;                                                                                     \
        q->length++;                                                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    T queue_##T##_pop(queue_##T* q) {                                                                                  \
        cstl_assert(q->length > 0);                                                                                    \
        const T result = q->buffer[q->front];                                                                          \
        q->front = (q->front + 1) % q->capacity;                                                                       \
        q->length--;                                                                                                   \
                                                                                                                       \
        return result;                                                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    bool queue_##T##_try_pop(queue_##T* q, T* out) {                                                                   \
        if (q->length > 0) {                                                                                           \
            const T result = queue_##T##_pop(q);                                                                       \
            if (out) {                                                                                                 \
                *out = result;                                                                                         \
            }                                                                                                          \
                                                                                                                       \
            return true;                                                                                               \
        } else {                                                                                                       \
            return false;                                                                                              \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    void queue_##T##_reserve(queue_##T* q, i64 new_capacity) {                                                         \
        cstl_assert(new_capacity >= 0);                                                                                \
        T* const new_buffer = cstl_alloc(T, new_capacity);                                                             \
        for (i64 i = 0; i < q->length; i++) {                                                                          \
            new_buffer[i] = q->buffer[(q->front + i) % q->capacity];                                                   \
        }                                                                                                              \
        cstl_free(T, q->buffer, q->capacity);                                                                          \
        q->buffer = new_buffer;                                                                                        \
        q->capacity = new_capacity;                                                                                    \
        q->front = 0;                                                                                                  \
        q->back = (q->length > 0) ? q->length - 1 : 0;                                                                 \
    }
