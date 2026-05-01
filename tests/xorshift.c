#include "CSTL/xorshift.h"
#include "CSTL/random.h"

void test_xorshift(void);

int main(void) {
    test_xorshift();

    return 0;
}

void test_xorshift(void) {
    xorshift x = xorshift_new(42);
    set_global_random(&x, xorshift_random_bytes);

    i32 a = 0;
    set_random_bytes(&a, sizeof(i32));
    cstl_assert(a == 1148610719);
    set_random_bytes(&a, sizeof(i32));
    cstl_assert(a == 1466906513);
    set_random_bytes(&a, sizeof(i32));
    cstl_assert(a == 203746700);
}
