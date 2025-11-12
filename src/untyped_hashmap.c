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

void UntypedHashmapFree(UntypedHashmap* map, Allocator allocator, I64 key_size, I64 key_align, I64 value_size,
                        I64 value_align) {
    AllocatorRawFree(allocator, (U8*)map->keys, key_size, map->capacity, key_align);
    AllocatorRawFree(allocator, (U8*)map->values, value_size, map->capacity, value_align);
    AllocatorFree(UntypedHashmapState, allocator, map->states, map->capacity);
}

void UntypedHashmapRealloc(UntypedHashmap* map, Allocator allocator, I64 key_size, I64 key_align, I64 value_size,
                           I64 value_align, I64 new_capacity) {
    void* new_keys = AllocatorRawAlloc(allocator, key_size, new_capacity, key_align);
    void* new_values = AllocatorRawAlloc(allocator, value_size, new_capacity, value_align);
    UntypedHashmapState* new_states = AllocatorAlloc(UntypedHashmapState, allocator, new_capacity);

    // Default to empty entries in case allocators do not zero memory
    memset(new_states, UNTYPED_HASHMAP_STATE_EMPTY, new_capacity * sizeof(UntypedHashmapState));

    if (map->count != 0) {
        // Rehash keys and values into new buffers
        for (I64 i = 0; i < map->capacity; i++) {
            if (map->states[i] != UNTYPED_HASHMAP_STATE_OCCUPIED) {
                continue;
            }
            const void* key = (U8*)map->keys + i * key_size;
            const void* value = (U8*)map->values + i * value_size;

            const I64 hash = map->hash(key);
            I64 index = hash % new_capacity;

            while (new_states[index] == UNTYPED_HASHMAP_STATE_OCCUPIED) {
                index = (index + 1) % new_capacity;
            }

            Assert(index < new_capacity);
            memcpy((U8*)new_keys + index * key_size, key, key_size);
            memcpy((U8*)new_values + index * value_size, value, value_size);
            new_states[index] = UNTYPED_HASHMAP_STATE_OCCUPIED;
        }
    }

    AllocatorRawFree(allocator, (U8*)map->keys, key_size, map->capacity, key_align);
    AllocatorRawFree(allocator, (U8*)map->values, value_size, map->capacity, value_align);
    AllocatorFree(UntypedHashmapState, allocator, map->states, map->capacity);

    map->keys = new_keys;
    map->values = new_values;
    map->states = new_states;
    map->capacity = new_capacity;
}

void UntypedHashmapSet(UntypedHashmap* map, Allocator allocator, I64 key_size, I64 key_align, const void* const key,
                       I64 value_size, I64 value_align, const void* const value) {
    // 80% load factor
    if (map->count * 10 >= map->capacity * 8) {
        // Larger starting capacity, prevent collisions
        const I64 new_capacity = (map->capacity == 0) ? 8 : map->capacity * 2;
        UntypedHashmapRealloc(map, allocator, key_size, key_align, value_size, value_align, new_capacity);
    }
    const I64 hash = map->hash(key);
    I64 index = hash % map->capacity;

    while (map->states[index] != UNTYPED_HASHMAP_STATE_EMPTY) {
        if (map->states[index] == UNTYPED_HASHMAP_STATE_OCCUPIED
            && map->compare(key, (U8*)map->keys + index * key_size)) {
            // Overwrite found value
            memcpy((U8*)map->values + index * value_size, value, value_size);
            return;
        }

        index = (index + 1) % map->capacity;
    }

    map->count++;
    memcpy((U8*)map->keys + index * key_size, key, key_size);
    memcpy((U8*)map->values + index * value_size, value, value_size);
    map->states[index] = UNTYPED_HASHMAP_STATE_OCCUPIED;
}

void* UntypedHashmapGet(UntypedHashmap* map, I64 key_size, const void* const key, I64 value_size) {
    if (map->capacity == 0) {
        return NULL;
    }

    const I64 hash = map->hash(key);
    I64 index = hash % map->capacity;

    while (map->states[index] != UNTYPED_HASHMAP_STATE_EMPTY) {
        if (map->states[index] == UNTYPED_HASHMAP_STATE_OCCUPIED
            && map->compare(key, (U8*)map->keys + index * key_size)) {
            Assert(index < map->capacity);

            return (U8*)map->values + index * value_size;
        }

        index = (index + 1) % map->capacity;
    }

    return NULL;
}

bool UntypedHashmapTryRemove(UntypedHashmap* map, I64 key_size, const void* const key, I64 value_size,
                             void** out_value) {
    if (map->capacity == 0) {
        if (out_value) {
            *out_value = NULL;

            return false;
        }
    }

    const I64 hash = map->hash(key);
    I64 index = hash % map->capacity;

    while (map->states[index] != UNTYPED_HASHMAP_STATE_EMPTY) {
        if (map->states[index] == UNTYPED_HASHMAP_STATE_OCCUPIED
            && map->compare(key, (U8*)map->keys + index * key_size)) {
            Assert(index < map->capacity);
            map->states[index] = UNTYPED_HASHMAP_STATE_TOMBSTONE;
            map->count--;

            if (out_value) {
                *out_value = (U8*)map->values + index * value_size;
            }

            return true;
        }

        index = (index + 1) % map->capacity;
    }

    if (out_value) {
        *out_value = NULL;
    }

    return false;
}

UntypedHashmapIterator UntypedHashmapIteratorNew(const UntypedHashmap* map) {
    UntypedHashmapIterator result = {0};
    result.inner = map;

    return result;
}

bool UntypedHashmapIteratorNext(UntypedHashmapIterator* it, I64 key_size, I64 value_size) {
    const UntypedHashmap* inner = it->inner;

    while (it->index < inner->capacity) {
        const I64 curr = it->index;
        it->index++;

        if (inner->states[curr] == UNTYPED_HASHMAP_STATE_OCCUPIED) {
            it->key = (U8*)inner->keys + curr * key_size;
            it->value = (U8*)inner->values + curr * value_size;

            return true;
        }
    }

    return false;
}
