#include "CSTL/untyped_set.h"
#include "CSTL/allocator.h"
#include <string.h>

untyped_set untyped_set_new(compare_func compare, hash_func hash) {
    untyped_set result = {0};
    result.compare = compare;
    result.hash = hash;

    return result;
}

void untyped_set_free(untyped_set* set, i64 item_size, i64 item_align) {
    cstl_raw_free(set->values, item_size, set->capacity, item_align);
    cstl_free(untyped_set_state, set->states, set->capacity);
}

void untyped_set_realloc(untyped_set* set, i64 item_size, i64 item_align, i64 new_capacity) {
    void* const new_values = cstl_raw_alloc(item_size, new_capacity, item_align);
    untyped_set_state* const new_states = cstl_alloc(untyped_set_state, new_capacity);

    memset(new_states, untyped_set_state_empty, new_capacity * sizeof(untyped_set_state));

    if (set->count != 0) {
        for (i64 i = 0; i < set->capacity; i++) {
            if (set->states[i] != untyped_set_state_occupied) {
                continue;
            }
            const void* value = (u8*)set->values + i * item_size;

            const i64 hash = set->hash(value);
            i64 index = hash % new_capacity;

            while (new_states[index] == untyped_set_state_occupied) {
                index = (index + 1) % new_capacity;
            }

            cstl_assert(index < new_capacity);
            memcpy((u8*)new_values + index * item_size, value, item_size);
            new_states[index] = untyped_set_state_occupied;
        }
    }

    cstl_raw_free(set->values, item_size, set->capacity, item_align);
    cstl_free(untyped_set_state, set->states, set->capacity);

    set->values = new_values;
    set->states = new_states;
    set->capacity = new_capacity;
    set->tombstone_count = 0;
}

void untyped_set_insert(untyped_set* set, i64 item_size, i64 item_align, void* elem) {
    if (set->count + set->tombstone_count >= (set->capacity / 10) * 8) {
        const i64 new_capacity = (set->capacity == 0) ? 8 : set->capacity * 2;
        untyped_set_realloc(set, item_size, new_capacity, item_align);
    }

    const i64 hash = set->hash(elem);
    i64 index = hash % set->capacity;
    i64 tombstone = -1;

    while (set->states[index] != untyped_set_state_empty) {
        if (set->states[index] == untyped_set_state_occupied
            && set->compare(elem, (u8*)set->values + index * item_size)) {
            return;
        } else if (set->states[index] == untyped_set_state_tombstone) {
            if (tombstone == -1) {
                tombstone = index;
            }
        }

        index = (index + 1) % set->capacity;
    }

    const i64 target = tombstone != -1 ? tombstone : index;
    if (target == tombstone) {
        set->tombstone_count--;
    }
    set->count++;
    memcpy((u8*)set->values + target * item_size, elem, item_size);
    set->states[target] = untyped_set_state_occupied;
}

bool untyped_set_contains(untyped_set set, i64 item_size, void* elem) {
    if (set.capacity == 0) {
        return false;
    }

    const i64 hash = set.hash(elem);
    i64 index = hash % set.capacity;

    while (set.states[index] != untyped_set_state_empty) {
        if (set.states[index] == untyped_set_state_occupied && set.compare(elem, (u8*)set.values + index * item_size)) {
            return true;
        }

        index = (index + 1) % set.capacity;
    }

    return false;
}

void* untyped_set_remove(untyped_set* set, i64 item_size, void* elem) {
    if (set->capacity == 0) {
        return NULL;
    }

    const i64 hash = set->hash(elem);
    i64 index = hash % set->capacity;

    while (set->states[index] != untyped_set_state_empty) {
        if (set->states[index] == untyped_set_state_occupied
            && set->compare(elem, (u8*)set->values + index * item_size)) {
            set->states[index] = untyped_set_state_tombstone;
            set->tombstone_count++;
            set->count--;

            return (u8*)set->values + index * item_size;
        }

        index = (index + 1) % set->capacity;
    }

    return NULL;
}

untyped_set_iterator untyped_set_iterator_new(const untyped_set* set) {
    untyped_set_iterator result = {0};
    result.inner = set;

    return result;
}

bool untyped_set_iterator_next(untyped_set_iterator* it, i64 item_size) {
    const untyped_set* const inner = it->inner;

    while (it->index < inner->capacity) {
        const i64 curr = it->index;
        it->index++;

        if (inner->states[curr] == untyped_set_state_occupied) {
            it->value = (u8*)inner->values + curr * item_size;

            return true;
        }
    }

    return false;
}
