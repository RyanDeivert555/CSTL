#pragma once

#define ARRAY_DEFINE(T, Size) \
    typedef struct array_##T { \
        T buffer[Size]; \
        size_t length; \
    } array_##T; \
    \
    array_##T array_##T##_new(void) { \
        array_##T result = {0}; \
        \
        return result; \
    }
