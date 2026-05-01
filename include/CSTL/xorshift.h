#pragma once
#include "common.h"

typedef struct xorshift {
    u64 state[4];
} xorshift;

xorshift xorshift_new(u64 seed);
void xorshift_random_bytes(void* ctx, void* buffer, i64 size);
