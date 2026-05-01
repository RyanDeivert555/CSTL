#include <CSTL/random.h>
#include <stdlib.h>

static void* global_ctx = NULL;
static random_bytes_func global_random_bytes = std_random_bytes;

void set_global_random(void* ctx, random_bytes_func random_bytes) {
    global_ctx = ctx;
    global_random_bytes = random_bytes;
}

void set_random_bytes(void* buffer, i64 size) {
    global_random_bytes(global_ctx, buffer, size);
}

void std_random_bytes(void* ctx, void* buffer, i64 size) {
    (void)ctx;

    for (i64 i = 0; i < size; i++) {
        // rand only guarenteed to get a max of 15 random bits, so go byte by byte
        ((u8*)buffer)[i] = rand() & 0xff;
    }
}
