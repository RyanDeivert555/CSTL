#pragma once
#include "allocator.h" // IWYU pragma: keep
#include "common.h"    // IWYU pragma: keep
#include <string.h>    // IWYU pragma: keep

#define cstl_hashmap(K, V) hashmap_##K##_##V
#define cstl_hashmap_create(K, V, alloc) hashmap_##K##_##V##_create(alloc)
#define cstl_hashmap_free(K, V, m) hashmap_##K##_##V##_free(m)
#define cstl_hashmap_get(K, V, m, key) hashmap_##K##_##V##_get(m, key)
#define cstl_hashmap_realloc(K, V, m, new_capacity) hashmap_##K##_##V##_realloc(m, new_capacity)
#define cstl_hashmap_set(K, V, m, key, value) hashmap_##K##_##V##_set(m, key, value)
#define cstl_hashmap_try_remove(K, V, m, key, out_value) hashmap_##K##_##V##_try_remove(m, key, out_value)

#define cstl_hashmap_iterator(K, V) hashmap_##K##_##V##_iterator
#define cstl_hashmap_iterator_new(K, V, m) hashmap_##K##_##V##_iterator_new(m)
#define cstl_hashmap_iterator_next(K, V, it) hashmap_##K##_##V##_iterator_next(it)

typedef enum hashmap_state {
    hashmap_state_empty,
    hashmap_state_occupied,
    hashmap_state_tombstone,
} hashmap_state;

typedef u8 hashmap_state_type;

#define HASHMAP_DEFINE(K, V)                                                                                           \
    typedef struct hashmap_##K##_##V {                                                                                 \
        K* keys;                                                                                                       \
        V* values;                                                                                                     \
        hashmap_state_type* states;                                                                                    \
        i64 capacity;                                                                                                  \
        i64 count;                                                                                                     \
        i64 tombstone_count;                                                                                           \
        allocator alloc;                                                                                               \
    } hashmap_##K##_##V;                                                                                               \
                                                                                                                       \
    hashmap_##K##_##V hashmap_##K##_##V##_create(allocator a);                                                         \
    void hashmap_##K##_##V##_free(hashmap_##K##_##V* map);                                                             \
    V* hashmap_##K##_##V##_get(hashmap_##K##_##V* map, K key);                                                         \
    void hashmap_##K##_##V##_realloc(hashmap_##K##_##V* map, i64 new_capacity);                                        \
    void hashmap_##K##_##V##_set(hashmap_##K##_##V* map, K key, V value);                                              \
    bool hashmap_##K##_##V##_try_remove(hashmap_##K##_##V* map, K key, V* out_value);                                  \
                                                                                                                       \
    typedef struct hashmap_##K##_##V##_iterator {                                                                      \
        const hashmap_##K##_##V* inner;                                                                                \
        K key;                                                                                                         \
        V value;                                                                                                       \
        i64 index;                                                                                                     \
    } hashmap_##K##_##V##_iterator;                                                                                    \
                                                                                                                       \
    hashmap_##K##_##V##_iterator hashmap_##K##_##V##_iterator_new(const hashmap_##K##_##V* map);                       \
    bool hashmap_##K##_##V##_iterator_next(hashmap_##K##_##V##_iterator* it);

#define HASHMAP_IMPL(K, V, compare_func, hash_func)                                                                    \
    hashmap_##K##_##V hashmap_##K##_##V##_create(allocator a) {                                                        \
        hashmap_##K##_##V instance = {0};                                                                              \
        instance.alloc = a;                                                                                            \
                                                                                                                       \
        return instance;                                                                                               \
    }                                                                                                                  \
                                                                                                                       \
    void hashmap_##K##_##V##_free(hashmap_##K##_##V* map) {                                                            \
        cstl_free(K, map->alloc, map->keys, map->capacity);                                                            \
        cstl_free(V, map->alloc, map->values, map->capacity);                                                          \
        cstl_free(hashmap_state_type, map->alloc, map->states, map->capacity);                                         \
        map->keys = NULL;                                                                                              \
        map->values = NULL;                                                                                            \
        map->states = NULL;                                                                                            \
        map->capacity = 0;                                                                                             \
        map->count = 0;                                                                                                \
        map->tombstone_count = 0;                                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    V* hashmap_##K##_##V##_get(hashmap_##K##_##V* map, K key) {                                                        \
        if (map->capacity == 0) {                                                                                      \
            return NULL;                                                                                               \
        }                                                                                                              \
                                                                                                                       \
        const u64 hash = hash_func(key);                                                                               \
        u64 index = hash & (u64)(map->capacity - 1);                                                                   \
                                                                                                                       \
        while (map->states[index] != hashmap_state_empty) {                                                            \
            if (map->states[index] == hashmap_state_occupied && compare_func(key, map->keys[index])) {                 \
                return &map->values[index];                                                                            \
            }                                                                                                          \
                                                                                                                       \
            index = (index + 1) & (u64)(map->capacity - 1);                                                            \
        }                                                                                                              \
                                                                                                                       \
        return NULL;                                                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    void hashmap_##K##_##V##_realloc(hashmap_##K##_##V* map, i64 new_capacity) {                                       \
        cstl_assert(new_capacity > map->count);                                                                        \
        new_capacity = cstl_next_pow2(new_capacity);                                                                   \
        if (new_capacity <= map->capacity) {                                                                           \
            return;                                                                                                    \
        }                                                                                                              \
                                                                                                                       \
        K* const new_keys = cstl_alloc(K, map->alloc, new_capacity);                                                   \
        V* const new_values = cstl_alloc(V, map->alloc, new_capacity);                                                 \
        hashmap_state_type* const new_states = cstl_alloc(hashmap_state_type, map->alloc, new_capacity);               \
        memset(new_states, hashmap_state_empty, new_capacity * sizeof(hashmap_state_type));                            \
                                                                                                                       \
        if (map->count != 0) {                                                                                         \
            for (i64 i = 0; i < map->capacity; i++) {                                                                  \
                if (map->states[i] != hashmap_state_occupied) {                                                        \
                    continue;                                                                                          \
                }                                                                                                      \
                const K key = map->keys[i];                                                                            \
                const V value = map->values[i];                                                                        \
                                                                                                                       \
                const u64 hash = hash_func(key);                                                                       \
                u64 index = hash & (u64)(new_capacity - 1);                                                            \
                                                                                                                       \
                while (new_states[index] == hashmap_state_occupied) {                                                  \
                    index = (index + 1) & (u64)(new_capacity - 1);                                                     \
                }                                                                                                      \
                                                                                                                       \
                new_keys[index] = key;                                                                                 \
                new_values[index] = value;                                                                             \
                new_states[index] = hashmap_state_occupied;                                                            \
            }                                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        cstl_free(K, map->alloc, map->keys, map->capacity);                                                            \
        cstl_free(V, map->alloc, map->values, map->capacity);                                                          \
        cstl_free(hashmap_state_type, map->alloc, map->states, map->capacity);                                         \
                                                                                                                       \
        map->keys = new_keys;                                                                                          \
        map->values = new_values;                                                                                      \
        map->states = new_states;                                                                                      \
        map->capacity = new_capacity;                                                                                  \
        map->tombstone_count = 0;                                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    void hashmap_##K##_##V##_set(hashmap_##K##_##V* map, K key, V value) {                                             \
        const u64 hash = hash_func(key);                                                                               \
                                                                                                                       \
        if (map->capacity > 0) {                                                                                       \
            u64 index = hash & (u64)(map->capacity - 1);                                                               \
                                                                                                                       \
            while (map->states[index] != hashmap_state_empty) {                                                        \
                if (map->states[index] == hashmap_state_occupied && compare_func(key, map->keys[index])) {             \
                    map->values[index] = value;                                                                        \
                    return;                                                                                            \
                }                                                                                                      \
                                                                                                                       \
                index = (index + 1) & (u64)(map->capacity - 1);                                                        \
            }                                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        if (map->count + map->tombstone_count >= (map->capacity * CSTL_LOAD_FACTOR) / 10) {                            \
            const i64 new_capacity = (map->capacity == 0) ? CSTL_INITIAL_CAPACITY : map->capacity * 2;                 \
            hashmap_##K##_##V##_realloc(map, new_capacity);                                                            \
        }                                                                                                              \
                                                                                                                       \
        u64 index = hash & (u64)(map->capacity - 1);                                                                   \
        i64 tombstone = -1;                                                                                            \
                                                                                                                       \
        while (map->states[index] != hashmap_state_empty) {                                                            \
            if (map->states[index] == hashmap_state_tombstone && tombstone == -1) {                                    \
                tombstone = index;                                                                                     \
            }                                                                                                          \
                                                                                                                       \
            index = (index + 1) & (u64)(map->capacity - 1);                                                            \
        }                                                                                                              \
                                                                                                                       \
        const i64 target = tombstone != -1 ? tombstone : (i64)index;                                                   \
        if (target == tombstone) {                                                                                     \
            map->tombstone_count--;                                                                                    \
        }                                                                                                              \
        map->count++;                                                                                                  \
        map->keys[target] = key;                                                                                       \
        map->values[target] = value;                                                                                   \
        map->states[target] = hashmap_state_occupied;                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    bool hashmap_##K##_##V##_try_remove(hashmap_##K##_##V* map, K key, V* out_value) {                                 \
        if (map->capacity == 0) {                                                                                      \
            return false;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        const u64 hash = hash_func(key);                                                                               \
        u64 index = hash & (u64)(map->capacity - 1);                                                                   \
                                                                                                                       \
        while (map->states[index] != hashmap_state_empty) {                                                            \
            if (map->states[index] == hashmap_state_occupied && compare_func(key, map->keys[index])) {                 \
                map->states[index] = hashmap_state_tombstone;                                                          \
                map->tombstone_count++;                                                                                \
                map->count--;                                                                                          \
                if (out_value) {                                                                                       \
                    *out_value = map->values[index];                                                                   \
                }                                                                                                      \
                                                                                                                       \
                return true;                                                                                           \
            }                                                                                                          \
                                                                                                                       \
            index = (index + 1) & (u64)(map->capacity - 1);                                                            \
        }                                                                                                              \
                                                                                                                       \
        return false;                                                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    hashmap_##K##_##V##_iterator hashmap_##K##_##V##_iterator_new(const hashmap_##K##_##V* map) {                      \
        hashmap_##K##_##V##_iterator result = {0};                                                                     \
        result.inner = map;                                                                                            \
                                                                                                                       \
        return result;                                                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    bool hashmap_##K##_##V##_iterator_next(hashmap_##K##_##V##_iterator* it) {                                         \
        const hashmap_##K##_##V* const inner = it->inner;                                                              \
                                                                                                                       \
        while (it->index < inner->capacity) {                                                                          \
            const i64 curr = it->index;                                                                                \
            it->index++;                                                                                               \
                                                                                                                       \
            if (inner->states[curr] == hashmap_state_occupied) {                                                       \
                it->key = inner->keys[curr];                                                                           \
                it->value = inner->values[curr];                                                                       \
                                                                                                                       \
                return true;                                                                                           \
            }                                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        return false;                                                                                                  \
    }
