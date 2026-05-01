#include "CSTL/untyped_vec.h"
#include <string.h>

void test_untyped_vec(void);

int main(void) {
    test_untyped_vec();

    return 0;
}
void test_untyped_vec(void) {
    untyped_vec vec = {0};
    const char* hello = "gdkkn";
    for (i64 i = 0; i < 5; i++) {
        untyped_vec_push(&vec, sizeof(u8), alignof(u8), &hello[i]);
    }

    for (i64 i = 0; i < vec.length; i++) {
        u8* item = (u8*)untyped_vec_get(&vec, i, sizeof(u8));
        *item += 1;
    }
    untyped_vec_push(&vec, sizeof(u8), alignof(u8), &(u8){'\n'});

    const u8* expected = (const u8*)"hello\n";
    for (i64 i = 0; i < vec.length; i++) {
        const u8 c = *(u8*)untyped_vec_get(&vec, i, sizeof(u8));
        cstl_assert(expected[i] == c);
    }

    untyped_vec_remove(&vec, vec.length - 1, sizeof(u8));

    const u8* last = untyped_vec_pop(&vec, sizeof(u8));
    cstl_assert(*last == 'o');

    const u8* first = untyped_vec_get(&vec, 0, sizeof(u8));
    cstl_assert(*first == 'h');

    untyped_vec_free(&vec, sizeof(u8), alignof(u8));

    // test with integers now
    memset(&vec, 0, sizeof(untyped_vec));
    cstl_assert(!vec.buffer && !vec.length && !vec.capacity);

    const i32 nums[] = {0, 1, 2, 4, 8, 16, -1};
    for (i64 i = 0; nums[i] != -1; i++) {
        untyped_vec_push(&vec, sizeof(i32), alignof(i32), &nums[i]);
    }

    i64 index = 5;
    while (vec.length != 0) {
        const i32* top = untyped_vec_pop(&vec, sizeof(i32));
        cstl_assert(*top == nums[index]);
        index--;
    }

    untyped_vec_free(&vec, sizeof(i32), alignof(i32));
}
