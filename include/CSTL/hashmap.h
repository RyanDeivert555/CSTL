#pragma once
#include "allocator.h" // IWYU pragma: keep
#include "common.h"    // IWYU pragma: keep
#include <string.h>    // IWYU pragma: keep

typedef enum hashmap_state {
    hashmap_state_empty,
    hashmap_state_occupied,
    hashmap_state_tombstone,
} hashmap_state;

#define HASHMAP_DEFINE(K, V)                                                                                           \
    typedef struct hashmap_##K##_##V {                                                                                 \
        K* keys;                                                                                                       \
        V* values;                                                                                                     \
        hashmap_state* states;                                                                                         \
        i64 capacity;                                                                                                  \
        i64 count;                                                                                                     \
        i64 tombstone_count;                                                                                           \
    } hashmap_##K##_##V;                                                                                               \
                                                                                                                       \
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
    void hashmap_##K##_##V##_free(hashmap_##K##_##V* map) {                                                            \
        cstl_free(K, map->keys, map->capacity);                                                                        \
        cstl_free(V, map->values, map->capacity);                                                                      \
        cstl_free(hashmap_state, map->states, map->capacity);                                                          \
    }                                                                                                                  \
                                                                                                                       \
    V* hashmap_##K##_##V##_get(hashmap_##K##_##V* map, K key) {                                                        \
        if (map->capacity == 0) {                                                                                      \
            return NULL;                                                                                               \
        }                                                                                                              \
                                                                                                                       \
        const i64 hash = hash_func(key);                                                                               \
        i64 index = hash % map->capacity;                                                                              \
                                                                                                                       \
        while (map->states[index] != hashmap_state_empty) {                                                            \
            if (map->states[index] == hashmap_state_occupied && compare_func(key, map->keys[index])) {                 \
                return &map->values[index];                                                                            \
            }                                                                                                          \
                                                                                                                       \
            index = (index + 1) % map->capacity;                                                                       \
        }                                                                                                              \
                                                                                                                       \
        return NULL;                                                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    void hashmap_##K##_##V##_realloc(hashmap_##K##_##V* map, i64 new_capacity) {                                       \
        K* const new_keys = cstl_alloc(K, new_capacity);                                                               \
        V* const new_values = cstl_alloc(V, new_capacity);                                                             \
        hashmap_state* const new_states = cstl_alloc(hashmap_state, new_capacity);                                     \
                                                                                                                       \
        memset(new_states, hashmap_state_empty, new_capacity * sizeof(hashmap_state));                                 \
                                                                                                                       \
        if (map->count != 0) {                                                                                         \
            for (i64 i = 0; i < map->capacity; i++) {                                                                  \
                if (map->states[i] != hashmap_state_occupied) {                                                        \
                    continue;                                                                                          \
                }                                                                                                      \
                const K key = map->keys[i];                                                                            \
                const V value = map->values[i];                                                                        \
                                                                                                                       \
                const i64 hash = hash_func(key);                                                                       \
                i64 index = hash % new_capacity;                                                                       \
                                                                                                                       \
                while (new_states[index] == hashmap_state_occupied) {                                                  \
                    index = (index + 1) % new_capacity;                                                                \
                }                                                                                                      \
                                                                                                                       \
                cstl_assert(index < new_capacity);                                                                     \
                new_keys[index] = key;                                                                                 \
                new_values[index] = value;                                                                             \
                new_states[index] = hashmap_state_occupied;                                                            \
            }                                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        cstl_free(K, map->keys, map->capacity);                                                                        \
        cstl_free(V, map->values, map->capacity);                                                                      \
        cstl_free(hashmap_state, map->states, map->capacity);                                                          \
                                                                                                                       \
        map->keys = new_keys;                                                                                          \
        map->values = new_values;                                                                                      \
        map->states = new_states;                                                                                      \
        map->capacity = new_capacity;                                                                                  \
        map->tombstone_count = 0;                                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    void hashmap_##K##_##V##_set(hashmap_##K##_##V* map, K key, V value) {                                             \
        if (map->count + map->tombstone_count >= (map->capacity / 10) * 8) {                                           \
            const i64 new_capacity = (map->capacity == 0) ? 8 : map->capacity * 2;                                     \
            hashmap_##K##_##V##_realloc(map, new_capacity);                                                            \
        }                                                                                                              \
                                                                                                                       \
        const i64 hash = hash_func(key);                                                                               \
        i64 index = hash % map->capacity;                                                                              \
        i64 tombstone = -1;                                                                                            \
                                                                                                                       \
        while (map->states[index] != hashmap_state_empty) {                                                            \
            if (map->states[index] == hashmap_state_occupied && compare_func(key, map->keys[index])) {                 \
                map->values[index] = value;                                                                            \
                return;                                                                                                \
            } else if (map->states[index] == hashmap_state_tombstone) {                                                \
                if (tombstone == -1) {                                                                                 \
                    tombstone = index;                                                                                 \
                }                                                                                                      \
            }                                                                                                          \
                                                                                                                       \
            index = (index + 1) % map->capacity;                                                                       \
        }                                                                                                              \
                                                                                                                       \
        const i64 target = tombstone != -1 ? tombstone : index;                                                        \
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
        const i64 hash = hash_func(key);                                                                               \
        i64 index = hash % map->capacity;                                                                              \
                                                                                                                       \
        while (map->states[index] != hashmap_state_empty) {                                                            \
            if (map->states[index] == hashmap_state_occupied && compare_func(key, map->keys[index])) {                 \
                map->states[index] = hashmap_state_tombstone;                                                          \
                map->tombstone_count++;                                                                                \
                map->count--;                                                                                          \
                if (out_value) {                                                                                       \
                    *out_value = map->values[index];                                                                   \
                                                                                                                       \
                    return true;                                                                                       \
                }                                                                                                      \
            }                                                                                                          \
                                                                                                                       \
            index = (index + 1) % map->capacity;                                                                       \
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
        const hashmap_##K##_##V* inner = it->inner;                                                                    \
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
