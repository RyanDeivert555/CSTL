#pragma once
#include "common.h"

typedef bool (*compare_func)(const void* lhs, const void* rhs);
typedef i64 (*hash_func)(const void* key);

typedef enum untyped_hashmap_state {
    untyped_hashmap_state_empty,
    untyped_hashmap_state_occupied,
    untyped_hashmap_state_tombstone,
} untyped_hashmap_state;

typedef struct untyped_hashmap {
    compare_func compare;
    hash_func hash;
    void* keys;
    void* values;
    untyped_hashmap_state* states;
    i64 capacity;
    i64 count;
    i64 tombstone_count;
} untyped_hashmap;

untyped_hashmap untyped_hashmap_new(compare_func compare, hash_func hash);
void untyped_hashmap_free(untyped_hashmap* map, i64 key_size, i64 key_align, i64 value_size, i64 value_align);
void untyped_hashmap_realloc(untyped_hashmap* map, i64 key_size, i64 key_align, i64 value_size, i64 value_align,
                             i64 new_size);
void untyped_hashmap_set(untyped_hashmap* map, i64 key_size, i64 key_align, const void* key, i64 value_size,
                         i64 value_align, const void* value);
void* untyped_hashmap_get(untyped_hashmap* map, i64 key_size, const void* key, i64 value_size);
bool untyped_hashmap_try_remove(untyped_hashmap* map, i64 key_size, const void* key, i64 value_size, void* out_value);

typedef struct untyped_hashmap_iterator {
    const untyped_hashmap* inner;
    void* key;
    void* value;
    i64 index;
} untyped_hashmap_iterator;

untyped_hashmap_iterator untyped_hashmap_iterator_new(const untyped_hashmap* map);
bool untyped_hashmap_iterator_next(untyped_hashmap_iterator* it, i64 key_size, i64 value_size);
