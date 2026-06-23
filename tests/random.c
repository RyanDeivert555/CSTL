#include "CSTL/random.h"
#include "CSTL/common.h"

void test_random(void);

int main(void) {
    test_random();

    return 0;
}

void test_random(void) {
    random_state rand = std_random_state(42);

    i32 a = 0;
    set_random_bytes(&rand, &a, sizeof(i32));
    cstl_assert(a == 691102790);
    set_random_bytes(&rand, &a, sizeof(i32));
    cstl_assert(a == -1662155164);
    set_random_bytes(&rand, &a, sizeof(i32));
    cstl_assert(a == 130162647);
}
