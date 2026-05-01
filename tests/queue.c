#include "CSTL/queue.h"

QUEUE_DEFINE(i32)
QUEUE_IMPL(i32)

void test_queue(void);

int main(void) {
    test_queue();

    return 0;
}

void test_queue(void) {
    queue_i32 q = {0};

    for (i32 i = 0; i < 10; i++) {
        queue_i32_push(&q, i);
    }
    cstl_assert(q.length == 10);

    for (i32 i = 0; i < 10; i++) {
        cstl_assert(queue_i32_pop(&q) == i);
    }
    cstl_assert(q.length == 0);

    for (i32 i = 0; i < 12; i++) {
        queue_i32_push(&q, i);
    }

    for (i32 i = 0; i < 8; i++) {
        cstl_assert(queue_i32_pop(&q) == i);
    }
    for (i32 i = 12; i < 20; i++) {
        queue_i32_push(&q, i);
    }
    cstl_assert(q.length == 12);

    for (i32 i = 20; i < 26; i++) {
        queue_i32_push(&q, i);
    }

    for (i32 i = 8; i < 26; i++) {
        i32 n = 0;
        cstl_assert(queue_i32_try_pop(&q, &n));
        cstl_assert(n == i);
    }
    cstl_assert(q.length == 0);

    cstl_assert(!queue_i32_try_pop(&q, NULL));
    cstl_assert(!queue_i32_try_pop(&q, NULL));

    queue_i32_free(&q);
}
