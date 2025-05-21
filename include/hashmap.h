#pragma once
#include "allocator.h" // IWYU pragma: keep
#include "common.h" // IWYU pragma: keep
#include <stdbool.h> // IWYU pragma: keep

// TODO: iterators for traversal

#define HASHMAP_DEFINE(K, V) \
    typedef struct Hashmap_##K##_##V##_Pair { \
        K key; \
        V value; \
        bool occupied; \
    } Hashmap_##K##_##V##_Pair; \
    \
    typedef struct Hashmap_##K##_##V { \
        Hashmap_##K##_##V##_Pair* entries; \
        i64 count; \
        i64 capacity; \
    } Hashmap_##K##_##V; \
    \
    Hashmap_##K##_##V Hashmap_##K##_##V##_New(void); \
    void Hashmap_##K##_##V##_Free(Hashmap_##K##_##V* map, Allocator allocator); \
    V* Hashmap_##K##_##V##_Get(Hashmap_##K##_##V* map, K key); \
    void Hashmap_##K##_##V##_Realloc(Hashmap_##K##_##V* map, Allocator allocator); \
    void Hashmap_##K##_##V##_Set(Hashmap_##K##_##V* map, Allocator allocator, K key, V value); \

#define HASHMAP_IMPL(K, V, HashFunc, EqlFunc) \
    Hashmap_##K##_##V Hashmap_##K##_##V##_New(void) { \
        Hashmap_##K##_##V result = {0}; \
        \
        return result; \
    } \
    \
    void Hashmap_##K##_##V##_Free(Hashmap_##K##_##V* map, Allocator allocator) { \
        AllocatorFree(Hashmap_##K##_##V##_Pair, allocator, map->entries, map->capacity); \
    } \
    \
    V* Hashmap_##K##_##V##_Get(Hashmap_##K##_##V* map, K key) { \
        if (map->capacity == 0) { \
            return NULL; \
        } \
        i64 index = HashFunc(key) % map->capacity; \
        \
        while (map->entries[index].occupied) { \
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
    static void Hashmap_##K##_##V##_SetEntry(Hashmap_##K##_##V##_Pair* entries, i64 capacity, K key, V value, i64* count) { \
        i64 index = HashFunc(key) % capacity; \
        \
        while (entries[index].occupied) { \
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
        entries[index].occupied = true; \
        if (count) { \
            (*count)++; \
        } \
    } \
    \
    void Hashmap_##K##_##V##_Realloc(Hashmap_##K##_##V* map, Allocator allocator) { \
        const i64 new_capacity = (map->capacity == 0) ? 50 : map->capacity * 2; \
        Hashmap_##K##_##V##_Pair* new_entries = AllocatorAlloc(Hashmap_##K##_##V##_Pair, allocator, new_capacity); \
        \
        for (i64 i = 0; i < map->capacity; i++) { \
            Hashmap_##K##_##V##_Pair* entry = &map->entries[i]; \
            if (entry->occupied) { \
                Hashmap_##K##_##V##_SetEntry(new_entries, new_capacity, entry->key, entry->value, NULL); \
            } \
        } \
        \
        AllocatorFree(Hashmap_##K##_##V##_Pair, allocator, map->entries, map->capacity); \
        map->entries = new_entries; \
        map->capacity = new_capacity; \
    } \
    \
    void Hashmap_##K##_##V##_Set(Hashmap_##K##_##V* map, Allocator allocator, K key, V value) { \
        if (map->count >= map->capacity / 2) { \
            Hashmap_##K##_##V##_Realloc(map, allocator); \
        } \
        Hashmap_##K##_##V##_SetEntry(map->entries, map->capacity, key, value, &map->count); \
    } \

