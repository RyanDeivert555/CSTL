#include "CSTL/untyped_queue.h"

void test_untyped_queue(void);

int main(void) {
    test_untyped_queue();

    return 0;
}

void test_untyped_queue(void) {
    untyped_queue q = {0};

    for (i32 i = 0; i < 10; i++) {
        untyped_queue_push(&q, sizeof(i32), alignof(i32), &i);
    }
    cstl_assert(q.length == 10);

    for (i32 i = 0; i < 10; i++) {
        i32* const res = untyped_queue_pop(&q, sizeof(i32));
        cstl_assert(res && *res == i);
    }
    cstl_assert(q.length == 0);

    for (i32 i = 0; i < 12; i++) {
        untyped_queue_push(&q, sizeof(i32), alignof(i32), &i);
    }

    for (i32 i = 0; i < 8; i++) {
        i32* const res = untyped_queue_pop(&q, sizeof(i32));
        cstl_assert(res && *res == i);
    }
    for (i32 i = 12; i < 20; i++) {
        untyped_queue_push(&q, sizeof(i32), alignof(i32), &i);
    }
    cstl_assert(q.length == 12);

    for (i32 i = 20; i < 26; i++) {
        untyped_queue_push(&q, sizeof(i32), alignof(i32), &i);
    }

    for (i32 i = 8; i < 26; i++) {
        i32 out = 0;
        cstl_assert(untyped_queue_try_pop(&q, sizeof(i32), &out));
        cstl_assert(out == i);
    }
    cstl_assert(q.length == 0);

    cstl_assert(!untyped_queue_try_pop(&q, sizeof(i32), NULL));
    cstl_assert(!untyped_queue_try_pop(&q, sizeof(i32), NULL));

    untyped_queue_free(&q, sizeof(i32), alignof(i32));
}
