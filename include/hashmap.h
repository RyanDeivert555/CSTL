#pragma once
#include "allocator.h" // IWYU pragma: keep
#include <stdlib.h> // IWYU pragma: keep

// TODO: iterators for traversal

#define HASHMAP_DEFINE(K, V) \
    typedef struct hashmap_##K##_##V##_pair { \
        K key; \
        V value; \
        int occupied; \
    } hashmap_##K##_##V##_pair; \
    \
    typedef struct hashmap_##K##_##V { \
        hashmap_##K##_##V##_pair* entries; \
        size_t count; \
        size_t capacity; \
        \
    } hashmap_##K##_##V; \
    \
    hashmap_##K##_##V hashmap_##K##_##V##_new(void); \
    void hashmap_##K##_##V##_free(allocator allocator, hashmap_##K##_##V* map); \
    V* hashmap_##K##_##V##_get(hashmap_##K##_##V* map, K key); \
    void hashmap_##K##_##V##_realloc(allocator allocator, hashmap_##K##_##V* map); \
    void hashmap_##K##_##V##_set(allocator allocator, hashmap_##K##_##V* map, K key, V value); \

#define HASHMAP_IMPL(K, V, HashFunc, EqlFunc) \
    hashmap_##K##_##V hashmap_##K##_##V##_new(void) { \
        hashmap_##K##_##V result = {0}; \
        \
        return result; \
    } \
    \
    void hashmap_##K##_##V##_free(allocator allocator, hashmap_##K##_##V* map) { \
        allocator_free(hashmap_##K##_##V##_pair, allocator, map->entries, map->capacity); \
    } \
    \
    V* hashmap_##K##_##V##_get(hashmap_##K##_##V* map, K key) { \
        size_t index = HashFunc(key) & (map->capacity - 1); \
        \
        while (map->entries[index].occupied == 1) { \
            if (EqlFunc(key, map->entries[index].key)) { \
                return &map->entries[index].value; \
            } \
            \
            index++; \
            if (index >= map->capacity) { \
                index = 0; \
            } \
        } \
        \
        return NULL; \
    } \
    \
    static void hashmap_##K##_##V##_set_entry(hashmap_##K##_##V##_pair* entries, size_t capacity, K key, V value, size_t* count) { \
        size_t index = HashFunc(key) & (capacity - 1); \
        \
        while (entries[index].occupied == 1) { \
            if (EqlFunc(key, entries[index].key)) { \
                entries[index].value = value; \
                \
                return; \
            } \
            \
            index++; \
            if (index >= capacity) { \
                index = 0; \
            } \
        } \
        \
        entries[index].key = key; \
        entries[index].value = value; \
        entries[index].occupied = 1; \
        if (count) { \
            (*count)++; \
        } \
    } \
    \
    void hashmap_##K##_##V##_realloc(allocator allocator, hashmap_##K##_##V* map) { \
        size_t new_capacity = (map->capacity == 0) ? 50 : map->capacity * 2; \
        \
        hashmap_##K##_##V##_pair* new_entries = allocator_alloc(hashmap_##K##_##V##_pair, allocator, new_capacity); \
        for (size_t i = 0; i < map->capacity; i++) { \
            hashmap_##K##_##V##_pair* entry = &map->entries[i]; \
            if (entry->occupied) { \
                hashmap_##K##_##V##_set_entry(new_entries, new_capacity, entry->key, entry->value, NULL); \
            } \
        } \
        \
        allocator_free(hashmap_##K##_##V##_pair, allocator, map->entries, map->capacity); \
        map->entries = new_entries; \
        map->capacity = new_capacity; \
    } \
    \
    void hashmap_##K##_##V##_set(allocator allocator, hashmap_##K##_##V* map, K key, V value) { \
        if (map->count >= map->capacity / 2) { \
            hashmap_##K##_##V##_realloc(allocator, map); \
        } \
        hashmap_##K##_##V##_set_entry(map->entries, map->capacity, key, value, &map->count); \
    } \

