#pragma once
#include "common.h"

typedef struct untyped_queue {
    void* buffer;
    i64 length;
    i64 capacity;
    i64 front;
    i64 back;
} untyped_queue;

void untyped_queue_free(untyped_queue* q, i64 item_size, i64 item_align);
void untyped_queue_push(untyped_queue* q, i64 item_size, i64 item_align, void* elem);
void* untyped_queue_pop(untyped_queue* q, i64 item_size);
bool untyped_queue_try_pop(untyped_queue* q, i64 item_size, void* out);
void untyped_queue_reserve(untyped_queue* q, i64 item_size, i64 item_capacity, i64 new_capacity);
