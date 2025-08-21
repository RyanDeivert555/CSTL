#include "untyped_hashmap.h"
#include "allocator.h"
#include "common.h"
#include <string.h>

UntypedHashmap UntypedHashmapNew(CompareFunc compare, HashFunc hash) {
    UntypedHashmap result = {0};
    result.compare = compare;
    result.hash = hash;

    return result;
}

void UntypedHashmapFree(UntypedHashmap* map, Allocator allocator, i64 key_size, i64 key_align, i64 value_size, i64 value_align) {
    AllocatorRawFree(allocator, (u8*)map->keys, key_size, map->capacity, key_align);
    AllocatorRawFree(allocator, (u8*)map->values, value_size, map->capacity, value_align);
    AllocatorFree(UntypedHashmapState, allocator, map->states, map->capacity);
}

void UntypedHashmapRealloc(UntypedHashmap* map, Allocator allocator, i64 key_size, i64 key_align, i64 value_size, i64 value_align, i64 new_capacity) {
    void* new_keys = AllocatorRawAlloc(allocator, key_size, new_capacity, key_align);
    void* new_values = AllocatorRawAlloc(allocator, value_size, new_capacity, value_align);
    UntypedHashmapState* new_states = AllocatorAlloc(UntypedHashmapState, allocator, new_capacity);

    // Default to empty entries in case allocators do not zero memory
    memset(new_states, 0, new_capacity * sizeof(UntypedHashmapState));

    if (map->count != 0) {
        // Rehash keys and values into new buffers
        for (i64 i = 0; i < map->capacity; i++) {
            if (map->states[i] == UNTYPED_HASHMAP_STATE_EMPTY) {
                continue;
            }
            const void* key = (u8*)map->keys + i * key_size;
            const void* value = (u8*)map->values + i * value_size;

            const i64 hash = map->hash(key);
            i64 index = hash % new_capacity;

            while (new_states[index] == UNTYPED_HASHMAP_STATE_OCCUPIED) {
                index = (index + 1) % new_capacity;
            }

            Assert(index < new_capacity);
            memcpy((u8*)new_keys + index * key_size, key, key_size);
            memcpy((u8*)new_values + index * value_size, value, value_size);
            new_states[index] = UNTYPED_HASHMAP_STATE_OCCUPIED;
        }
    }

    AllocatorRawFree(allocator, (u8*)map->keys, key_size, map->capacity, key_align);
    AllocatorRawFree(allocator, (u8*)map->values, value_size, map->capacity, value_align);
    AllocatorFree(UntypedHashmapState, allocator, map->states, map->capacity);

    map->keys = new_keys;
    map->values = new_values;
    map->states = new_states;
    map->capacity = new_capacity;
}

void UntypedHashmapSet(UntypedHashmap* map, Allocator allocator, i64 key_size, i64 key_align, const void* const key, i64 value_size, i64 value_align, const void* const value) {
    // 80% load factor
    if (map->count * 10 >= map->capacity * 8) {
        // Larger starting capacity, prevent collisions
        const i64 new_capacity = (map->capacity == 0) ? 8 : map->capacity * 2;
        UntypedHashmapRealloc(map, allocator, key_size, key_align, value_size, value_align, new_capacity);
    }
    const i64 hash = map->hash(key);
    i64 index = hash % map->capacity;

    while (map->states[index] != UNTYPED_HASHMAP_STATE_EMPTY) {
        if (map->states[index] == UNTYPED_HASHMAP_STATE_OCCUPIED && map->compare(key, (u8*)map->keys + index * key_size)) {
            // Overwrite found value
            memcpy((u8*)map->values + index * value_size, value, value_size);
            return;
        }

        index = (index + 1) % map->capacity;
    }

    map->count++;
    memcpy((u8*)map->keys + index * key_size, key, key_size);
    memcpy((u8*)map->values + index * value_size, value, value_size);
    map->states[index] = UNTYPED_HASHMAP_STATE_OCCUPIED;
}

void* UntypedHashmapGet(UntypedHashmap* map, i64 key_size, const void* const key, i64 value_size) {
    if (map->capacity == 0) {
        return NULL;
    }

    const i64 hash = map->hash(key);
    i64 index = hash % map->capacity;

    while (map->states[index] != UNTYPED_HASHMAP_STATE_EMPTY) {
        if (map->states[index] == UNTYPED_HASHMAP_STATE_OCCUPIED && map->compare(key, (u8*)map->keys + index * key_size)) {
            Assert(index < map->capacity);

            return (u8*)map->values + index * value_size;
        }

        index = (index + 1) % map->capacity;
    }

    return NULL;
}

