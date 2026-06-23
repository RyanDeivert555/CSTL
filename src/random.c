#include <CSTL/random.h>
#include <stdlib.h>

void set_random_bytes(random_state* rand, void* buffer, i64 size) {
    rand->rand(rand->ctx, buffer, size);
}

void std_random_bytes(void* ctx, void* buffer, i64 size) {
    (void)ctx;

    for (i64 i = 0; i < size; i++) {
        // rand only guarenteed to get a max of 15 random bits, so go byte by byte
        ((u8*)buffer)[i] = rand() & 0xff;
    }
}

static random_state std_rand = {
    .ctx = NULL,
    .rand = std_random_bytes,
};

random_state std_random_state(u64 seed) {
    srand(seed);

    return std_rand;
}
