#include "CSTL/random.h"

void test_random(void);

int main(void) {
    test_random();

    return 0;
}

void test_random(void) {
    random_state rand = shared_random_state(42);

    i32 a = 0;
    set_random_bytes(rand, &a, sizeof(i32));
    cstl_assert(a == 1148610719);
    shared_random_set_bytes(&a, sizeof(i32));
    cstl_assert(a == 1466906513);
    set_random_bytes(rand, &a, sizeof(i32));
    cstl_assert(a == 203746700);
    shared_random_set_bytes(&a, sizeof(i32));
    cstl_assert(a == 215496120);
    set_random_bytes(rand, &a, sizeof(i32));
    cstl_assert(a == 1954835059);
    shared_random_set_bytes(&a, sizeof(i32));
    cstl_assert(a == 1313922685);
}
