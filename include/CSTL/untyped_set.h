#pragma once
#include "common.h"

typedef bool (*compare_func)(const void* lhs, const void* rhs);
typedef i64 (*hash_func)(const void* key);

typedef enum untyped_set_state : u8 {
    untyped_set_state_empty,
    untyped_set_state_occupied,
    untyped_set_state_tombstone,
} untyped_set_state;

typedef struct untyped_set {
    void* values;
    untyped_set_state* states;
    compare_func compare;
    hash_func hash;
    i64 capacity;
    i64 count;
    i64 tombstone_count;
} untyped_set;

untyped_set untyped_set_new(compare_func compare, hash_func hash);
void untyped_set_free(untyped_set* set, i64 item_size, i64 item_align);
void untyped_set_realloc(untyped_set* set, i64 item_size, i64 item_align, i64 new_capacity);
void untyped_set_insert(untyped_set* set, i64 item_size, i64 item_align, void* elem);
bool untyped_set_contains(untyped_set set, i64 item_size, void* elem);
void* untyped_set_remove(untyped_set* set, i64 item_size, void* elem);

typedef struct untyped_set_iterator {
    const untyped_set* inner;
    void* value;
    i64 index;
} untyped_set_iterator;

untyped_set_iterator untyped_set_iterator_new(const untyped_set* set);
bool untyped_set_iterator_next(untyped_set_iterator* it, i64 item_size);
