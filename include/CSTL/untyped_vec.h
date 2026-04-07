#pragma once
#include "common.h"

typedef struct untyped_vec {
    void* buffer;
    i64 length;
    i64 capacity;
} untyped_vec;

void untyped_vec_free(untyped_vec* vec, i64 item_size, i64 item_align);
void untyped_vec_push(untyped_vec* vec, i64 item_size, i64 item_align, const void* const item);
void* untyped_vec_pop(untyped_vec* vec, i64 item_size);
void untyped_vec_insert(untyped_vec* vec, i64 index, i64 item_size, i64 item_align, const void* const item);
void untyped_vec_remove(untyped_vec* vec, i64 index, i64 item_size);
void untyped_vec_reserve(untyped_vec* vec, i64 item_size, i64 item_align, i64 new_capacity);
void* untyped_vec_get(untyped_vec* vec, i64 index, i64 item_size);
void untyped_vec_set(untyped_vec* vec, i64 index, i64 item_size, const void* const item);

#define vec_free(T, vec) untyped_vec_free(vec, sizeof(T), alignof(T))
#define vec_push(T, vec, item) untyped_vec_push(vec, sizeof(T), alignof(T), item)
#define vec_pop(T, vec) untyped_vec_pop(vec, sizeof(T))
#define vec_insert(T, vec, index, item) untyped_vec_insert(vec, index, sizeof(T), alignof(T), item)
#define vec_remove(T, vec, index) untyped_vec_remove(vec, index, sizeof(T))
#define vec_reserve(T, vec, new_capacity) untyped_vec_reserve(vec, sizeof(T), alignof(T), new_capacity)
#define vec_get(T, vec, index) (T*)untyped_vec_get(vec, index, sizeof(T))
#define vec_set(T, vec, index, item) untyped_vec_set(vec, index, sizeof(T), item)
