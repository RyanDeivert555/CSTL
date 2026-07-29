#pragma once
#include "common.h"

typedef void (*random_bytes_func)(void* ctx, void* buffer, i64 size);

typedef struct random_state {
    void* ctx;
    random_bytes_func rand;
} random_state;

void set_random_bytes(random_state* rand, void* buffer, i64 size);
random_state shared_random_state(u64 seed);
