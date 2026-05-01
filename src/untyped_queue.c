#include "CSTL/untyped_queue.h"
#include "CSTL/allocator.h"
#include "CSTL/common.h"
#include <string.h>

void untyped_queue_free(untyped_queue* q, i64 item_size, i64 item_align) {
    cstl_raw_free(q->buffer, item_size, q->capacity, item_align);
}

void untyped_queue_push(untyped_queue* q, i64 item_size, i64 item_align, void* elem) {
    if (q->length == q->capacity) {
        const i64 new_capacity = q->capacity == 0 ? 1 : q->capacity * 2;
        untyped_queue_reserve(q, item_size, item_align, new_capacity);
    }

    if (q->length == 0) {
        q->front = 0;
        q->back = 0;
    } else {
        q->back = (q->back + 1) % q->capacity;
    }

    memcpy((u8*)q->buffer + q->back * item_size, elem, item_size);
    q->length++;
}

void* untyped_queue_pop(untyped_queue* q, i64 item_size) {
    cstl_assert(q->length > 0);
    void* const result = (u8*)q->buffer + q->front * item_size;
    q->front = (q->front + 1) % q->capacity;
    q->length--;

    return result;
}

bool untyped_queue_try_pop(untyped_queue* q, i64 item_size, void* out) {
    if (q->length > 0) {
        const void* const result = untyped_queue_pop(q, item_size);
        if (out) {
            memcpy(out, result, item_size);
        }

        return true;
    } else {
        return false;
    }
}

void untyped_queue_reserve(untyped_queue* q, i64 item_size, i64 item_align, i64 new_capacity) {
    cstl_assert(new_capacity >= 0);
    void* const new_buffer = cstl_raw_alloc(item_size, new_capacity, item_align);
    for (i64 i = 0; i < q->length; i++) {
        const void* const item = (u8*)q->buffer + (((q->front + i) % q->capacity) * item_size);
        memcpy((u8*)new_buffer + i * item_size, item, item_size);
    }
    cstl_raw_free(q->buffer, item_size, q->capacity, item_align);
    q->buffer = new_buffer;
    q->capacity = new_capacity;
    q->front = 0;
    q->back = (q->length > 0) ? q->length - 1 : 0;
}
