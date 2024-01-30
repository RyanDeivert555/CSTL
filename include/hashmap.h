#pragma once
#include <assert.h>

#define HASHMAP_DEFINE(TBufferSize, T, K) \
    typedef struct hashmap_##T##_to_##K { \
        T buffer[TBufferSize]; \
        size_t (*hasher)(T); \
    } hashmap_##T##_to_##K; \
    \
    hashmap_##T##_to_##K hashmap_##T##_to_##K##_new(size_t (*hasher)(T)) { \
        hashmap_##T##_to_##K result = {0}; \
        result.hasher = hasher; \
        \
        return result; \
    } \
    \
    void hashmap_##T##_to_##K##_insert(hashmap_##T##_to_##K* map, T key, K value) { \
        size_t index = map->hasher(key); \
        assert(index < TBufferSize); \
        map->buffer[index] = value; \
    } \
    \
    K hashmap_##T##_to_##K##_get(hashmap_##T##_to_##K map, T key) { \
        size_t index = map.hasher(key); \
        assert(index < TBufferSize); \
        \
        return map.buffer[index]; \
    }
    