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

    if (map->count != 0) {
        // Default to empty entries
        memset(new_states, STATE_EMPTY, new_capacity * sizeof(UntypedHashmapState));
        memcpy(new_keys, map->keys, map->capacity * key_size);
        // TODO: rehash instead of memcpy
        memcpy(new_values, map->values, map->capacity * value_size);
        memmove(new_states, map->states, map->capacity * sizeof(UntypedHashmapState));
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
    if (map->count >= map->capacity / 2) {
        const i64 new_capacity = (map->capacity == 0) ? 1 : map->capacity * 2;
        UntypedHashmapRealloc(map, allocator, key_size, key_align, value_size, value_align, new_capacity);
    }
    const i64 hash = map->hash(key);
    i64 index = hash % map->capacity;

    while (map->states[index] == STATE_OCCUPIED) {
        if (map->compare(key, (u8*)map->keys + index * key_size)) {
            // Overwrite found value
            memmove((u8*)map->values + index * value_size, value, value_size);
            return;
        }

        index++;
        if (index >= map->capacity) {
            index = 0;
        }
    }

    map->count++;
    memmove((u8*)map->keys + index * key_size, key, key_size);
    memmove((u8*)map->values + index * value_size, value, value_size);
    map->states[index] = STATE_OCCUPIED;
}

void* UntypedHashmapGet(UntypedHashmap* map, i64 key_size, const void* const key, i64 value_size) {
    if (map->capacity == 0) {
        return NULL;
    }

    const i64 hash = map->hash(key);
    i64 index = hash % map->capacity;

    while (map->states[index] == STATE_OCCUPIED) {
        if (map->compare(key, (u8*)map->keys + index * key_size)) {
            return (u8*)map->values + index * value_size;
        }

        index++;
        if (index >= map->capacity) {
            index = 0;
        }
    }

    return NULL;
}

