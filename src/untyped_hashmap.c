#include "CSTL/untyped_hashmap.h"
#include "CSTL/allocator.h"
#include "CSTL/common.h"
#include <string.h>

untyped_hashmap untyped_hashmap_new(compare_func compare, hash_func hash) {
    untyped_hashmap result = {0};
    result.compare = compare;
    result.hash = hash;

    return result;
}

void untyped_hashmap_free(untyped_hashmap* map, i64 key_size, i64 key_align, i64 value_size, i64 value_align) {
    cstl_raw_free((u8*)map->keys, key_size, map->capacity, key_align);
    cstl_raw_free((u8*)map->values, value_size, map->capacity, value_align);
    cstl_free(untyped_hashmap_state, map->states, map->capacity);
}

void untyped_hashmap_realloc(untyped_hashmap* map, i64 key_size, i64 key_align, i64 value_size, i64 value_align,
                             i64 new_capacity) {
    void* const new_keys = cstl_raw_alloc(key_size, new_capacity, key_align);
    void* const new_values = cstl_raw_alloc(value_size, new_capacity, value_align);
    untyped_hashmap_state* const new_states = cstl_alloc(untyped_hashmap_state, new_capacity);

    // Default to empty entries in case allocators do not zero memory
    memset(new_states, untyped_hashmap_state_empty, new_capacity * sizeof(untyped_hashmap_state));

    if (map->count != 0) {
        // Rehash keys and values into new buffers
        for (i64 i = 0; i < map->capacity; i++) {
            if (map->states[i] != untyped_hashmap_state_occupied) {
                continue;
            }
            const void* key = (u8*)map->keys + i * key_size;
            const void* value = (u8*)map->values + i * value_size;

            const i64 hash = map->hash(key);
            i64 index = hash % new_capacity;

            while (new_states[index] == untyped_hashmap_state_occupied) {
                index = (index + 1) % new_capacity;
            }

            cstl_assert(index < new_capacity);
            memcpy((u8*)new_keys + index * key_size, key, key_size);
            memcpy((u8*)new_values + index * value_size, value, value_size);
            new_states[index] = untyped_hashmap_state_occupied;
        }
    }

    cstl_raw_free((u8*)map->keys, key_size, map->capacity, key_align);
    cstl_raw_free((u8*)map->values, value_size, map->capacity, value_align);
    cstl_free(untyped_hashmap_state, map->states, map->capacity);

    map->keys = new_keys;
    map->values = new_values;
    map->states = new_states;
    map->capacity = new_capacity;
}

void untyped_hashmap_set(untyped_hashmap* map, i64 key_size, i64 key_align, const void* const key, i64 value_size,
                         i64 value_align, const void* const value) {
    // 80% load factor
    if (map->count * 10 >= map->capacity * 8) {
        // Larger starting capacity, prevent collisions
        const i64 new_capacity = (map->capacity == 0) ? 8 : map->capacity * 2;
        untyped_hashmap_realloc(map, key_size, key_align, value_size, value_align, new_capacity);
    }
    const i64 hash = map->hash(key);
    i64 index = hash % map->capacity;

    while (map->states[index] != untyped_hashmap_state_empty) {
        if (map->states[index] == untyped_hashmap_state_occupied
            && map->compare(key, (u8*)map->keys + index * key_size)) {
            // Overwrite found value
            memcpy((u8*)map->values + index * value_size, value, value_size);
            return;
        }

        index = (index + 1) % map->capacity;
    }

    map->count++;
    memcpy((u8*)map->keys + index * key_size, key, key_size);
    memcpy((u8*)map->values + index * value_size, value, value_size);
    map->states[index] = untyped_hashmap_state_occupied;
}

void* untyped_hashmap_get(untyped_hashmap* map, i64 key_size, const void* const key, i64 value_size) {
    if (map->capacity == 0) {
        return NULL;
    }

    const i64 hash = map->hash(key);
    i64 index = hash % map->capacity;

    while (map->states[index] != untyped_hashmap_state_empty) {
        if (map->states[index] == untyped_hashmap_state_occupied
            && map->compare(key, (u8*)map->keys + index * key_size)) {
            cstl_assert(index < map->capacity);

            return (u8*)map->values + index * value_size;
        }

        index = (index + 1) % map->capacity;
    }

    return NULL;
}

bool untyped_hashmap_try_remove(untyped_hashmap* map, i64 key_size, const void* const key, i64 value_size,
                                void** out_value) {
    if (map->capacity == 0) {
        if (out_value) {
            *out_value = NULL;

            return false;
        }
    }

    const i64 hash = map->hash(key);
    i64 index = hash % map->capacity;

    while (map->states[index] != untyped_hashmap_state_empty) {
        if (map->states[index] == untyped_hashmap_state_occupied
            && map->compare(key, (u8*)map->keys + index * key_size)) {
            cstl_assert(index < map->capacity);
            map->states[index] = untyped_hashmap_state_tombstone;
            map->count--;

            if (out_value) {
                *out_value = (u8*)map->values + index * value_size;
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

untyped_hashmap_iterator untyped_hashmap_iterator_new(const untyped_hashmap* map) {
    untyped_hashmap_iterator result = {0};
    result.inner = map;

    return result;
}

bool untyped_hashmap_iterator_next(untyped_hashmap_iterator* it, i64 key_size, i64 value_size) {
    const untyped_hashmap* inner = it->inner;

    while (it->index < inner->capacity) {
        const i64 curr = it->index;
        it->index++;

        if (inner->states[curr] == untyped_hashmap_state_occupied) {
            it->key = (u8*)inner->keys + curr * key_size;
            it->value = (u8*)inner->values + curr * value_size;

            return true;
        }
    }

    return false;
}
