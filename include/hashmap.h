#pragma once
#include "allocator.h" // IWYU pragma: keep
#include "common.h" // IWYU pragma: keep
#include <stdbool.h> // IWYU pragma: keep
#include <string.h> // IWYU pragma: keep

typedef enum HashmapState {
    HASHMAP_STATE_EMPTY,
    HASHMAP_STATE_OCCUPIED,
    HASHMAP_STATE_TOMBSTONE,
} HashmapState;

#define HASHMAP_DEFINE(K, V) \
    typedef struct Hashmap_##K##_##V { \
        K* keys; \
        V* values; \
        HashmapState* states; \
        i64 capacity; \
        i64 count; \
    } Hashmap_##K##_##V; \
    \
    void Hashmap_##K##_##V##_Free(Hashmap_##K##_##V* map, Allocator allocator); \
    V* Hashmap_##K##_##V##_Get(Hashmap_##K##_##V* map, K key); \
    void Hashmap_##K##_##V##_Realloc(Hashmap_##K##_##V* map, Allocator allocator, i64 new_capacity); \
    void Hashmap_##K##_##V##_Set(Hashmap_##K##_##V* map, Allocator allocator, K key, V value); \
    bool Hashmap_##K##_##V##_TryRemove(Hashmap_##K##_##V* map, K key, V* out_value); \
    \
    typedef struct Hashmap_##K##_##V##_Iterator { \
        const Hashmap_##K##_##V* inner; \
        K key; \
        V value; \
        i64 index; \
    } Hashmap_##K##_##V##_Iterator; \
    \
    Hashmap_##K##_##V##_Iterator Hashmap_##K##_##V##_IteratorNew(const Hashmap_##K##_##V* map); \
    bool Hashmap_##K##_##V##_IteratorNext(Hashmap_##K##_##V##_Iterator* it); \

#define HASHMAP_IMPL(K, V, CompareFunc, HashFunc) \
    void Hashmap_##K##_##V##_Free(Hashmap_##K##_##V* map, Allocator allocator) { \
        AllocatorFree(K, allocator, map->keys, map->capacity); \
        AllocatorFree(V, allocator, map->values, map->capacity); \
        AllocatorFree(HashmapState, allocator, map->states, map->capacity); \
    } \
    \
    V* Hashmap_##K##_##V##_Get(Hashmap_##K##_##V* map, K key) { \
        if (map->capacity == 0) { \
            return NULL; \
        } \
        \
        const i64 hash = HashFunc(key); \
        i64 index = hash % map->capacity; \
        \
        while (map->states[index] != HASHMAP_STATE_EMPTY) { \
            if (map->states[index] == HASHMAP_STATE_OCCUPIED && CompareFunc(key, map->keys[index])) { \
                return &map->values[index]; \
            } \
            \
            index = (index + 1) % map->capacity; \
        } \
        \
        return NULL; \
    } \
    \
    void Hashmap_##K##_##V##_Realloc(Hashmap_##K##_##V* map, Allocator allocator, i64 new_capacity) { \
        K* new_keys = AllocatorAlloc(K, allocator, new_capacity); \
        V* new_values = AllocatorAlloc(V, allocator, new_capacity); \
        HashmapState* new_states = AllocatorAlloc(HashmapState, allocator, new_capacity); \
        \
        memset(new_states, 0, new_capacity * sizeof(HashmapState)); \
        \
        if (map->count != 0) { \
            for (i64 i = 0; i < map->capacity; i++) { \
                if (map->states[i] == HASHMAP_STATE_EMPTY) { \
                    continue; \
                } \
                const K key = map->keys[i]; \
                const V value = map->values[i]; \
                \
                const i64 hash = HashFunc(key); \
                i64 index = hash % map->capacity; \
                \
                while (new_states[index] == HASHMAP_STATE_OCCUPIED) { \
                    index = (index + 1) % new_capacity; \
                } \
                \
                Assert(index < new_capacity); \
                new_keys[index] = key; \
                new_values[index] = value; \
                new_states[index] = HASHMAP_STATE_OCCUPIED; \
            } \
        } \
        \
        AllocatorFree(K, allocator, map->keys, map->capacity); \
        AllocatorFree(V, allocator, map->values, map->capacity); \
        AllocatorFree(HashmapState, allocator, map->states, map->capacity); \
        \
        map->keys = new_keys; \
        map->values = new_values; \
        map->states = new_states; \
        map->capacity = new_capacity; \
    } \
    \
    void Hashmap_##K##_##V##_Set(Hashmap_##K##_##V* map, Allocator allocator, K key, V value) { \
        if (map->count * 10 >= map->capacity * 8) { \
            const i64 new_capacity = (map->capacity == 0) ? 8 : map->capacity * 2; \
            Hashmap_##K##_##V##_Realloc(map, allocator, new_capacity); \
        } \
        \
        const i64 hash = HashFunc(key); \
        i64 index = hash % map->capacity; \
        \
        while (map->states[index] != HASHMAP_STATE_EMPTY) { \
            if (map->states[index] == HASHMAP_STATE_OCCUPIED && CompareFunc(key, map->keys[index])) { \
                map->values[index] = value; \
                return; \
            } \
            \
            index = (index + 1) % map->capacity; \
        } \
        \
        map->count++; \
        map->keys[index] = key; \
        map->values[index] = value; \
        map->states[index] = HASHMAP_STATE_OCCUPIED; \
    } \
    \
    bool Hashmap_##K##_##V##_TryRemove(Hashmap_##K##_##V* map, K key, V* out_value) { \
        if (map->capacity == 0) { \
            return false; \
        } \
        \
        const i64 hash = HashFunc(key); \
        i64 index = hash % map->capacity; \
        \
        while (map->states[index] != HASHMAP_STATE_EMPTY) { \
            if (map->states[index] == HASHMAP_STATE_OCCUPIED && CompareFunc(key, map->keys[index])) { \
                map->states[index] = HASHMAP_STATE_TOMBSTONE; \
                if (out_value) { \
                    *out_value = map->values[index]; \
                    \
                    return true; \
                } \
            } \
            \
            index = (index + 1) % map->capacity; \
        } \
        \
        return false; \
    } \
    \
    Hashmap_##K##_##V##_Iterator Hashmap_##K##_##V##_IteratorNew(const Hashmap_##K##_##V* map) { \
        Hashmap_##K##_##V##_Iterator result = {0}; \
        result.inner = map; \
        \
        return result; \
    } \
    \
    bool Hashmap_##K##_##V##_IteratorNext(Hashmap_##K##_##V##_Iterator* it) { \
        const Hashmap_##K##_##V* inner = it->inner; \
        \
        while (it->index < inner->capacity) { \
            const i64 curr = it->index; \
            it->index++; \
            \
            if (inner->states[curr] == HASHMAP_STATE_OCCUPIED) { \
                it->key = inner->keys[curr]; \
                it->value = inner->values[curr]; \
                \
                return true; \
            } \
        } \
        \
        return false; \
    } \

