#pragma once
#include <assert.h>

#define BOUNDED_ARRAY_DEFINE(T, Size) \
    typedef struct bounded_array_##T##_size_##Size { \
        T data[Size]; \
        size_t length; \
    } bounded_array_##T##_size_##Size; \
    \
    bounded_array_##T##_size_##Size bounded_array_##T##_size_##Size##_new(void); \
    void bounded_array_##T##_size_##Size##_append(bounded_array_##T##_size_##Size* array, T entry); \

#define BOUNDED_ARRAY_IMPLEMENTATION(T, Size) \
    bounded_array_##T##_size_##Size bounded_array_##T##_size_##Size##_new(void) { \
        bounded_array_##T##_size_##Size result = {0}; \
        result.length = 0; \
        \
        return result; \
    } \
    void bounded_array_##T##_size_##Size##_append(bounded_array_##T##_size_##Size* array, T entry) { \
        assert(array->length < Size); \
        array->data[array->length] = entry; \
        array->length++; \
    }
