#pragma once
#include "allocator.h" // IWYU pragma: keep
#include "common.h"    // IWYU pragma: keep
#include <string.h>    // IWYU pragma: keep

#define cstl_queue(T) queue_##T
#define cstl_queue_create(T, alloc) queue_##T##_create(alloc)
#define cstl_queue_free(T, q) queue_##T##_free(q)
#define cstl_queue_push(T, q, elem) queue_##T##_push(q, elem)
#define cstl_queue_pop(T, q) queue_##T##_pop(q)
#define cstl_queue_try_pop(T, q, out) queue_##T##_try_pop(q, out)
#define cstl_queue_reserve(T, q, new_capacity) queue_##T##_reserve(q, new_capacity)

#define QUEUE_DEFINE(T)                                                                                                \
    typedef struct queue_##T {                                                                                         \
        T* buffer;                                                                                                     \
        i64 length;                                                                                                    \
        i64 capacity;                                                                                                  \
        i64 front;                                                                                                     \
        i64 back;                                                                                                      \
        allocator alloc;                                                                                               \
    } queue_##T;                                                                                                       \
                                                                                                                       \
    queue_##T queue_##T##_create(allocator a);                                                                         \
    void queue_##T##_free(queue_##T* q);                                                                               \
    void queue_##T##_push(queue_##T* q, T elem);                                                                       \
    T queue_##T##_pop(queue_##T* q);                                                                                   \
    bool queue_##T##_try_pop(queue_##T* q, T* out);                                                                    \
    void queue_##T##_reserve(queue_##T* q, i64 new_capacity);

#define QUEUE_IMPL(T)                                                                                                  \
    queue_##T queue_##T##_create(allocator a) {                                                                        \
        queue_##T instance = {0};                                                                                      \
        instance.alloc = a;                                                                                            \
                                                                                                                       \
        return instance;                                                                                               \
    }                                                                                                                  \
                                                                                                                       \
    void queue_##T##_free(queue_##T* q) {                                                                              \
        cstl_free(T, q->alloc, q->buffer, q->capacity);                                                                \
        q->buffer = NULL;                                                                                              \
        q->length = 0;                                                                                                 \
        q->capacity = 0;                                                                                               \
        q->front = 0;                                                                                                  \
        q->back = 0;                                                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    void queue_##T##_push(queue_##T* q, T elem) {                                                                      \
        if (q->length == q->capacity) {                                                                                \
            const i64 new_capacity = q->capacity == 0 ? CSTL_INITIAL_CAPACITY : q->capacity * 2;                       \
            queue_##T##_reserve(q, new_capacity);                                                                      \
        }                                                                                                              \
                                                                                                                       \
        if (q->length == 0) {                                                                                          \
            q->front = 0;                                                                                              \
            q->back = 0;                                                                                               \
        } else {                                                                                                       \
            q->back = (q->back + 1) & (q->capacity - 1);                                                               \
        }                                                                                                              \
                                                                                                                       \
        q->buffer[q->back] = elem;                                                                                     \
        q->length++;                                                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    T queue_##T##_pop(queue_##T* q) {                                                                                  \
        cstl_assert(q->length > 0);                                                                                    \
        const T result = q->buffer[q->front];                                                                          \
        q->front = (q->front + 1) & (q->capacity - 1);                                                                 \
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
        cstl_assert(new_capacity >= q->length);                                                                        \
        new_capacity = cstl_next_pow2(new_capacity);                                                                   \
        if (new_capacity <= q->capacity) {                                                                             \
            return;                                                                                                    \
        }                                                                                                              \
                                                                                                                       \
        T* const new_buffer = cstl_alloc(T, q->alloc, new_capacity);                                                   \
        for (i64 i = 0; i < q->length; i++) {                                                                          \
            new_buffer[i] = q->buffer[(q->front + i) & (q->capacity - 1)];                                             \
        }                                                                                                              \
                                                                                                                       \
        cstl_free(T, q->alloc, q->buffer, q->capacity);                                                                \
        q->buffer = new_buffer;                                                                                        \
        q->capacity = new_capacity;                                                                                    \
        q->front = 0;                                                                                                  \
        q->back = (q->length > 0) ? q->length - 1 : 0;                                                                 \
    }
