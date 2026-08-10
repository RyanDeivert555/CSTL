#include "CSTL/xorshift.h"
#include "CSTL/random.h"

void test_xorshift(void);

int main(void) {
    test_xorshift();

    return 0;
}

void test_xorshift(void) {
    xorshift_state x = xorshift_new(42);
    random_state rand = xorshift_as_random(&x);

    i32 a = 0;
    set_random_bytes(rand, &a, sizeof(i32));
    cstl_assert(a == 1148610719);
    set_random_bytes(rand, &a, sizeof(i32));
    cstl_assert(a == 1466906513);
    set_random_bytes(rand, &a, sizeof(i32));
    cstl_assert(a == 203746700);
    set_random_bytes(rand, &a, sizeof(i32));
    cstl_assert(a == 215496120);
    set_random_bytes(rand, &a, sizeof(i32));
    cstl_assert(a == 1954835059);
    set_random_bytes(rand, &a, sizeof(i32));
    cstl_assert(a == 1313922685);
}
