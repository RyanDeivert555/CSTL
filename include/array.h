#pragma once

#define ARRAY_DEFINE(T, TSize) \
    typedef struct array_##T { \
        T buffer[TSize]; \
        size_t length; \
    } array_##T; \
    \
    array_##T array_##T##_new(void) { \
        array_##T result = {0}; \
        \
        return result; \
    }
