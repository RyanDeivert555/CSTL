#pragma once
#include <assert.h>

#define BOUNDED_ARRAY_DEFINE(T, Size) \
    typedef struct bounded_array_##T { \
        T data[Size]; \
        size_t length; \
    } bounded_array_##T; \
    \
    bounded_array_##T bounded_array_##T##_new(void) { \
        bounded_array_##T result = {0}; \
        result.length = 0; \
        \
        return result; \
    } \
    void bounded_array_##T##_append(bounded_array_##T* array, T entry) { \
        assert(array->length < Size); \
        array->data[array->length] = entry; \
        array->length++; \
    }
