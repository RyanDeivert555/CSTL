#include "CSTL/xorshift.h"
#include <string.h>

static u64 rol64(u64 x, i32 k) {
    return (x << k) | (x >> (64 - k));
}

static u64 xorshift_next(xorshift* instance) {
    const u64 result = rol64(instance->state[0] + instance->state[3], 23) + instance->state[0];
    const u64 t = instance->state[1] << 17;

    instance->state[2] ^= instance->state[0];
    instance->state[3] ^= instance->state[1];
    instance->state[1] ^= instance->state[2];
    instance->state[0] ^= instance->state[3];

    instance->state[2] ^= t;
    instance->state[3] = rol64(instance->state[3], 45);

    return result;
}

static u64 splitmix64(u64* state) {
    *state += 0x9E3779B97F4A7C15;

    u64 curr = *state;
    curr = (curr ^ (curr >> 30)) * 0xBF58476D1CE4E5B9;
    curr = (curr ^ (curr >> 27)) * 0x94D049BB133111EB;

    return curr ^ (curr >> 31);
}

xorshift xorshift_new(u64 seed) {
    xorshift result = {0};

    result.state[0] = splitmix64(&seed);
    result.state[1] = splitmix64(&seed);
    result.state[2] = splitmix64(&seed);
    result.state[3] = splitmix64(&seed);

    return result;
}

void xorshift_random_bytes(void* ctx, void* buffer, i64 size) {
    xorshift* const instance = ctx;

    i64 i = 0;
    while (i + (i64)sizeof(u64) <= size) {
        const u64 next = xorshift_next(instance);

        memcpy((u8*)buffer + i, &next, sizeof(u64));

        i += sizeof(u64);
    }

    if (i < size) {
        const u64 next = xorshift_next(instance);
        memcpy((u8*)buffer + i, &next, size - i);
    }
}
