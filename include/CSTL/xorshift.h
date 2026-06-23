#pragma once
#include "common.h"
#include "random.h"

typedef struct xorshift_state {
    u64 state[4];
} xorshift_state;

xorshift_state xorshift_new(u64 seed);
random_state xorshift_as_random(xorshift_state* xorshift);
void xorshift_random_bytes(void* ctx, void* buffer, i64 size);
