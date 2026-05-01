#pragma once
#include "common.h"

typedef struct untyped_vec {
    void* buffer;
    i64 length;
    i64 capacity;
} untyped_vec;

void untyped_vec_free(untyped_vec* vec, i64 item_size, i64 item_align);
void untyped_vec_push(untyped_vec* vec, i64 item_size, i64 item_align, const void* item);
void* untyped_vec_pop(untyped_vec* vec, i64 item_size);
void untyped_vec_insert(untyped_vec* vec, i64 index, i64 item_size, i64 item_align, const void* item);
void untyped_vec_remove(untyped_vec* vec, i64 index, i64 item_size);
void untyped_vec_reserve(untyped_vec* vec, i64 item_size, i64 item_align, i64 new_capacity);
void* untyped_vec_get(untyped_vec* vec, i64 index, i64 item_size);
void untyped_vec_set(untyped_vec* vec, i64 index, i64 item_size, const void* item);
