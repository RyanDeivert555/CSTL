#pragma once
#include "allocator.h" // IWYU pragma: keep
#include "common.h"    // IWYU pragma: keep
#include <string.h>    // IWYU pragma: keep

typedef enum set_state : u8 {
    set_state_empty,
    set_state_occupied,
    set_state_tombstone,
} set_state;

#define SET_DEFINE(T)                                                                                                  \
    typedef struct set_##T {                                                                                           \
        T* values;                                                                                                     \
        set_state* states;                                                                                             \
        i64 capacity;                                                                                                  \
        i64 count;                                                                                                     \
        i64 tombstone_count;                                                                                           \
    } set_##T;                                                                                                         \
                                                                                                                       \
    void set_##T##_free(set_##T* set);                                                                                 \
    void set_##T##_realloc(set_##T* set, i64 new_capacity);                                                            \
    void set_##T##_insert(set_##T* set, T elem);                                                                       \
    bool set_##T##_contains(set_##T set, T elem);                                                                      \
    T* set_##T##_remove(set_##T* set, T elem);                                                                         \
                                                                                                                       \
    typedef struct set_##T##_iterator {                                                                                \
        const set_##T* inner;                                                                                          \
        T value;                                                                                                       \
        i64 index;                                                                                                     \
    } set_##T##_iterator;                                                                                              \
                                                                                                                       \
    set_##T##_iterator set_##T##_iterator_new(const set_##T* set);                                                     \
    bool set_##T##_iterator_next(set_##T##_iterator* it);

#define SET_IMPL(T, compare_func, hash_func)                                                                           \
    void set_##T##_free(set_##T* set) {                                                                                \
        cstl_free(T, set->values, set->capacity);                                                                      \
        cstl_free(set_state, set->states, set->capacity);                                                              \
    }                                                                                                                  \
                                                                                                                       \
    void set_##T##_realloc(set_##T* set, i64 new_capacity) {                                                           \
        T* const new_values = cstl_alloc(T, new_capacity);                                                             \
        set_state* const new_states = cstl_alloc(set_state, new_capacity);                                             \
                                                                                                                       \
        memset(new_states, set_state_empty, new_capacity * sizeof(set_state));                                         \
                                                                                                                       \
        if (set->count != 0) {                                                                                         \
            for (i64 i = 0; i < set->capacity; i++) {                                                                  \
                if (set->states[i] != set_state_occupied) {                                                            \
                    continue;                                                                                          \
                }                                                                                                      \
                const T value = set->values[i];                                                                        \
                                                                                                                       \
                const i64 hash = hash_func(value);                                                                     \
                i64 index = hash % new_capacity;                                                                       \
                                                                                                                       \
                while (new_states[index] == set_state_occupied) {                                                      \
                    index = (index + 1) % new_capacity;                                                                \
                }                                                                                                      \
                                                                                                                       \
                cstl_assert(index < new_capacity);                                                                     \
                new_values[index] = value;                                                                             \
                new_states[index] = set_state_occupied;                                                                \
            }                                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        cstl_free(T, set->values, set->capacity);                                                                      \
        cstl_free(set_state, set->states, set->capacity);                                                              \
                                                                                                                       \
        set->values = new_values;                                                                                      \
        set->states = new_states;                                                                                      \
        set->capacity = new_capacity;                                                                                  \
        set->tombstone_count = 0;                                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    void set_##T##_insert(set_##T* set, T elem) {                                                                      \
        if (set->count + set->tombstone_count >= (set->capacity / 10) * 8) {                                           \
            const i64 new_capacity = (set->capacity == 0) ? 8 : set->capacity * 2;                                     \
            set_##T##_realloc(set, new_capacity);                                                                      \
        }                                                                                                              \
                                                                                                                       \
        const i64 hash = hash_func(elem);                                                                              \
        i64 index = hash % set->capacity;                                                                              \
        i64 tombstone = -1;                                                                                            \
                                                                                                                       \
        while (set->states[index] != set_state_empty) {                                                                \
            if (set->states[index] == set_state_occupied && compare_func(elem, set->values[index])) {                  \
                return;                                                                                                \
            } else if (set->states[index] == set_state_tombstone) {                                                    \
                if (tombstone == -1) {                                                                                 \
                    tombstone = index;                                                                                 \
                }                                                                                                      \
            }                                                                                                          \
                                                                                                                       \
            index = (index + 1) % set->capacity;                                                                       \
        }                                                                                                              \
                                                                                                                       \
        const i64 target = tombstone != -1 ? tombstone : index;                                                        \
        if (target == tombstone) {                                                                                     \
            set->tombstone_count--;                                                                                    \
        }                                                                                                              \
        set->count++;                                                                                                  \
        set->values[target] = elem;                                                                                    \
        set->states[target] = set_state_occupied;                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    bool set_##T##_contains(set_##T set, T elem) {                                                                     \
        if (set.capacity == 0) {                                                                                       \
            return false;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        const i64 hash = hash_func(elem);                                                                              \
        i64 index = hash % set.capacity;                                                                               \
                                                                                                                       \
        while (set.states[index] != set_state_empty) {                                                                 \
            if (set.states[index] == set_state_occupied && compare_func(elem, set.values[index])) {                    \
                return true;                                                                                           \
            }                                                                                                          \
                                                                                                                       \
            index = (index + 1) % set.capacity;                                                                        \
        }                                                                                                              \
                                                                                                                       \
        return false;                                                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    T* set_##T##_remove(set_##T* set, T elem) {                                                                        \
        if (set->capacity == 0) {                                                                                      \
            return NULL;                                                                                               \
        }                                                                                                              \
                                                                                                                       \
        const i64 hash = hash_func(elem);                                                                              \
        i64 index = hash % set->capacity;                                                                              \
                                                                                                                       \
        while (set->states[index] != set_state_empty) {                                                                \
            if (set->states[index] == set_state_occupied && compare_func(elem, set->values[index])) {                  \
                set->states[index] = set_state_tombstone;                                                              \
                set->tombstone_count++;                                                                                \
                set->count--;                                                                                          \
                                                                                                                       \
                return &set->values[index];                                                                            \
            }                                                                                                          \
                                                                                                                       \
            index = (index + 1) % set->capacity;                                                                       \
        }                                                                                                              \
                                                                                                                       \
        return NULL;                                                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    set_##T##_iterator set_##T##_iterator_new(const set_##T* set) {                                                    \
        set_##T##_iterator result = {0};                                                                               \
        result.inner = set;                                                                                            \
                                                                                                                       \
        return result;                                                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    bool set_##T##_iterator_next(set_##T##_iterator* it) {                                                             \
        const set_##T* const inner = it->inner;                                                                        \
                                                                                                                       \
        while (it->index < inner->capacity) {                                                                          \
            const i64 curr = it->index;                                                                                \
            it->index++;                                                                                               \
                                                                                                                       \
            if (inner->states[curr] == set_state_occupied) {                                                           \
                it->value = inner->values[curr];                                                                       \
                                                                                                                       \
                return true;                                                                                           \
            }                                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        return false;                                                                                                  \
    }
