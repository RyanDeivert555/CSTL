#include "CSTL/random.h"
#include "CSTL/xorshift.h"
#include <stdatomic.h>
#include <threads.h>

void set_random_bytes(random_state rand, void* buffer, i64 size) {
    cstl_assert(size >= 0);
    rand.rand(rand.ctx, buffer, size);
}

static xorshift_state global_shared_state = {0};
static mtx_t rand_lock = {0};
static once_flag init_flag = ONCE_FLAG_INIT;
static atomic_uint_fast64_t global_seed = 0;

static void once_init(void) {
    global_shared_state = xorshift_new(global_seed);
    mtx_init(&rand_lock, mtx_plain);
}

static void shared_random_bytes(void* ctx, void* buffer, i64 size) {
    xorshift_state* const instance = ctx;
    const random_state rand = xorshift_as_random(instance);

    mtx_lock(&rand_lock);
    rand.rand(rand.ctx, buffer, size);
    mtx_unlock(&rand_lock);
}

static void ensure_initialized(void) {
    call_once(&init_flag, once_init);
}

void shared_random_set_seed(u64 seed) {
    global_seed = seed;
    ensure_initialized();
}

void shared_random_set_bytes(void* buffer, i64 size) {
    ensure_initialized();
    shared_random_bytes(&global_shared_state, buffer, size);
}

random_state shared_random_state(u64 seed) {
    shared_random_set_seed(seed);

    return (random_state){
        .ctx = &global_shared_state,
        .rand = shared_random_bytes,
    };
}
