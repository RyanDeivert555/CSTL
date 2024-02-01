#pragma once
#include <assert.h>

#define ARRAY_HASHMAP_DEFINE(TBufferSize, K, V) \
    typedef struct array_hashmap_##K##_to_##V { \
        V buffer[TBufferSize]; \
        size_t length; \
        size_t (*hasher)(K); \
    } array_hashmap_##K##_to_##V; \
    \
    array_hashmap_##K##_to_##V array_hashmap_##K##_to_##V##_new(size_t (*hasher)(K)) { \
        array_hashmap_##K##_to_##V result = {0}; \
        result.length = TBufferSize; \
        result.hasher = hasher; \
        \
        return result; \
    } \
    \
    size_t array_hashmap_##K##_to_##V##_hash(array_hashmap_##K##_to_##V map, K key) { \
        return map.hasher(key) % map.length; \
    } \
    \
    void array_hashmap_##K##_to_##V##_insert(array_hashmap_##K##_to_##V* map, K key, V value) { \
        size_t index = array_hashmap_##K##_to_##V##_hash(*map, key); \
        assert(index < map->length); \
        map->buffer[index] = value; \
    } \
    \
    V array_hashmap_##K##_to_##V##_get(array_hashmap_##K##_to_##V map, K key) { \
        size_t index = array_hashmap_##K##_to_##V##_hash(map, key); \
        assert(index < map.length); \
        \
        return map.buffer[index]; \
    }
