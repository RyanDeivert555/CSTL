#pragma once
#include <assert.h>

#define HASHMAP_DEFINE(TBufferSize, K, V) \
    typedef struct hashmap_##K##_to_##V { \
        V buffer[TBufferSize]; \
        size_t (*hasher)(K); \
    } hashmap_##K##_to_##V; \
    \
    hashmap_##K##_to_##V hashmap_##K##_to_##V##_new(size_t (*hasher)(K)) { \
        hashmap_##K##_to_##V result = {0}; \
        result.hasher = hasher; \
        \
        return result; \
    } \
    \
    void hashmap_##K##_to_##V##_insert(hashmap_##K##_to_##V* map, K key, V value) { \
        size_t index = map->hasher(key); \
        assert(index < TBufferSize); \
        map->buffer[index] = value; \
    } \
    \
    V hashmap_##K##_to_##V##_get(hashmap_##K##_to_##V map, K key) { \
        size_t index = map.hasher(key); \
        assert(index < TBufferSize); \
        \
        return map.buffer[index]; \
    }
