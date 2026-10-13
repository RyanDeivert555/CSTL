#pragma once
#include "allocator.h" // IWYU pragma: keep
#include "common.h"    // IWYU pragma: keep
#include <string.h>    // IWYU pragma: keep

#define cstl_set(T) set_##T
#define cstl_set_create(T, alloc) set_##T##_create(alloc)
#define cstl_set_free(T, s) set_##T##_free(s)
#define cstl_set_realloc(T, s, new_capacity) set_##T##_reserve(s, new_capacity)
#define cstl_set_insert(T, s, elem) set_##T##_insert(s, elem)
#define cstl_set_contains(T, s, elem) set_##T##_contains(s, elem)
#define cstl_set_remove(T, s, elem) set_##T##_remove(s, elem)

#define cstl_set_iterator(T) set_##T##_iterator
#define cstl_set_iterator_new(T, s) set_##T##_iterator_new(s)
#define cstl_set_iterator_next(T, it) set_##T##_iterator_next(it)

typedef enum set_state {
    set_state_empty,
    set_state_occupied,
    set_state_tombstone,
} set_state;

typedef u8 set_state_type;

#define SET_DEFINE(T)                                                                                                  \
    typedef struct set_##T {                                                                                           \
        T* values;                                                                                                     \
        set_state_type* states;                                                                                        \
        isize capacity;                                                                                                \
        isize count;                                                                                                   \
        isize tombstone_count;                                                                                         \
        allocator alloc;                                                                                               \
    } set_##T;                                                                                                         \
                                                                                                                       \
    set_##T set_##T##_create(allocator a);                                                                             \
    void set_##T##_free(set_##T* set);                                                                                 \
    void set_##T##_reserve(set_##T* set, isize new_capacity);                                                          \
    void set_##T##_insert(set_##T* set, T elem);                                                                       \
    bool set_##T##_contains(set_##T set, T elem);                                                                      \
    bool set_##T##_remove(set_##T* set, T elem);                                                                       \
                                                                                                                       \
    typedef struct set_##T##_iterator {                                                                                \
        const set_##T* inner;                                                                                          \
        T value;                                                                                                       \
        isize index;                                                                                                   \
    } set_##T##_iterator;                                                                                              \
                                                                                                                       \
    set_##T##_iterator set_##T##_iterator_new(const set_##T* set);                                                     \
    bool set_##T##_iterator_next(set_##T##_iterator* it);

#define SET_IMPL(T, compare_func, hash_func)                                                                           \
    set_##T set_##T##_create(allocator a) {                                                                            \
        set_##T instance = {0};                                                                                        \
        instance.alloc = a;                                                                                            \
                                                                                                                       \
        return instance;                                                                                               \
    }                                                                                                                  \
                                                                                                                       \
    void set_##T##_free(set_##T* set) {                                                                                \
        cstl_free(T, set->alloc, set->values, set->capacity);                                                          \
        cstl_free(set_state_type, set->alloc, set->states, set->capacity);                                             \
        set->values = NULL;                                                                                            \
        set->states = NULL;                                                                                            \
        set->capacity = 0;                                                                                             \
        set->count = 0;                                                                                                \
        set->tombstone_count = 0;                                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    void set_##T##_reserve(set_##T* set, isize new_capacity) {                                                         \
        cstl_assert(new_capacity > set->count);                                                                        \
        new_capacity = cstl_next_pow2(new_capacity);                                                                   \
        if (new_capacity <= set->capacity) {                                                                           \
            return;                                                                                                    \
        }                                                                                                              \
                                                                                                                       \
        T* const new_values = cstl_alloc(T, set->alloc, new_capacity);                                                 \
        set_state_type* const new_states = cstl_alloc(set_state_type, set->alloc, new_capacity);                       \
        memset(new_states, set_state_empty, new_capacity * sizeof(set_state_type));                                    \
                                                                                                                       \
        if (set->count != 0) {                                                                                         \
            for (isize i = 0; i < set->capacity; i++) {                                                                \
                if (set->states[i] != set_state_occupied) {                                                            \
                    continue;                                                                                          \
                }                                                                                                      \
                const T value = set->values[i];                                                                        \
                                                                                                                       \
                const usize hash = hash_func(value);                                                                   \
                usize index = hash & (usize)(new_capacity - 1);                                                        \
                                                                                                                       \
                while (new_states[index] == set_state_occupied) {                                                      \
                    index = (index + 1) & (usize)(new_capacity - 1);                                                   \
                }                                                                                                      \
                                                                                                                       \
                new_values[index] = value;                                                                             \
                new_states[index] = set_state_occupied;                                                                \
            }                                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        cstl_free(T, set->alloc, set->values, set->capacity);                                                          \
        cstl_free(set_state_type, set->alloc, set->states, set->capacity);                                             \
                                                                                                                       \
        set->values = new_values;                                                                                      \
        set->states = new_states;                                                                                      \
        set->capacity = new_capacity;                                                                                  \
        set->tombstone_count = 0;                                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    void set_##T##_insert(set_##T* set, T elem) {                                                                      \
        const usize hash = hash_func(elem);                                                                            \
                                                                                                                       \
        if (set->capacity > 0) {                                                                                       \
            usize index = hash & (usize)(set->capacity - 1);                                                           \
                                                                                                                       \
            while (set->states[index] != set_state_empty) {                                                            \
                if (set->states[index] == set_state_occupied && compare_func(elem, set->values[index])) {              \
                    return;                                                                                            \
                }                                                                                                      \
                                                                                                                       \
                index = (index + 1) & (usize)(set->capacity - 1);                                                      \
            }                                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        if (set->count + set->tombstone_count >= (set->capacity * CSTL_LOAD_FACTOR) / 10) {                            \
            const isize new_capacity = (set->capacity == 0) ? CSTL_INITIAL_CAPACITY : set->capacity * 2;               \
            set_##T##_reserve(set, new_capacity);                                                                      \
        }                                                                                                              \
                                                                                                                       \
        usize index = hash & (usize)(set->capacity - 1);                                                               \
        isize tombstone = -1;                                                                                          \
                                                                                                                       \
        while (set->states[index] != set_state_empty) {                                                                \
            if (set->states[index] == set_state_tombstone && tombstone == -1) {                                        \
                tombstone = index;                                                                                     \
            }                                                                                                          \
                                                                                                                       \
            index = (index + 1) & (usize)(set->capacity - 1);                                                          \
        }                                                                                                              \
                                                                                                                       \
        const isize target = tombstone != -1 ? tombstone : (isize)index;                                               \
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
        const usize hash = hash_func(elem);                                                                            \
        usize index = hash & (usize)(set.capacity - 1);                                                                \
                                                                                                                       \
        while (set.states[index] != set_state_empty) {                                                                 \
            if (set.states[index] == set_state_occupied && compare_func(elem, set.values[index])) {                    \
                return true;                                                                                           \
            }                                                                                                          \
                                                                                                                       \
            index = (index + 1) & (usize)(set.capacity - 1);                                                           \
        }                                                                                                              \
                                                                                                                       \
        return false;                                                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    bool set_##T##_remove(set_##T* set, T elem) {                                                                      \
        if (set->capacity == 0) {                                                                                      \
            return false;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        const usize hash = hash_func(elem);                                                                            \
        usize index = hash & (usize)(set->capacity - 1);                                                               \
                                                                                                                       \
        while (set->states[index] != set_state_empty) {                                                                \
            if (set->states[index] == set_state_occupied && compare_func(elem, set->values[index])) {                  \
                set->states[index] = set_state_tombstone;                                                              \
                set->tombstone_count++;                                                                                \
                set->count--;                                                                                          \
                                                                                                                       \
                return true;                                                                                           \
            }                                                                                                          \
                                                                                                                       \
            index = (index + 1) & (usize)(set->capacity - 1);                                                          \
        }                                                                                                              \
                                                                                                                       \
        return false;                                                                                                  \
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
            const isize curr = it->index;                                                                              \
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
