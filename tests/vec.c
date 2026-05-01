#include "CSTL/vec.h"

VEC_DEFINE(i32)
VEC_IMPL(i32)

void test_vec(void);

int main(void) {
    test_vec();

    return 0;
}

void test_vec(void) {
    vec_i32 v = {0};

    vec_i32_reserve(&v, 10);
    cstl_assert(v.capacity >= 10);
    cstl_assert(v.length == 0);

    for (i32 i = 0; i < 10000; i++) {
        vec_i32_push(&v, i);
    }
    cstl_assert(v.length == 10000);
    cstl_assert(v.capacity >= 10000);
    cstl_assert(v.buffer[0] == 0);
    cstl_assert(v.buffer[5000] == 5000);
    cstl_assert(v.buffer[9999] == 9999);

    const i32 popped = vec_i32_pop(&v);
    cstl_assert(popped == 9999);
    cstl_assert(v.length == 9999);

    vec_i32_insert(&v, 777, 0);
    cstl_assert(v.length == 10000);
    cstl_assert(v.buffer[0] == 777);
    cstl_assert(v.buffer[1] == 0);

    vec_i32_insert(&v, 888, 5000);
    cstl_assert(v.length == 10001);
    cstl_assert(v.buffer[5000] == 888);
    cstl_assert(v.buffer[5001] == 4999);

    vec_i32_insert(&v, 999, v.length);
    cstl_assert(v.length == 10002);
    cstl_assert(v.buffer[10001] == 999);

    const i32 removed_front = vec_i32_remove(&v, 0);
    cstl_assert(removed_front == 777);
    cstl_assert(v.length == 10001);
    cstl_assert(v.buffer[0] == 0);

    const i32 removed_mid = vec_i32_remove(&v, 4999);
    cstl_assert(removed_mid == 888);
    cstl_assert(v.length == 10000);
    cstl_assert(v.buffer[4999] == 4999);

    const i32 removed_end = vec_i32_remove(&v, v.length - 1);
    cstl_assert(removed_end == 999);
    cstl_assert(v.length == 9999);

    vec_i32_free(&v);
}
