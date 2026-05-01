#include "CSTL/allocator.h"

void test_allocator(void);

int main(void) {
    test_allocator();

    return 0;
}

void test_allocator(void) {
    {
        i32* const num = (i32*)cstl_raw_alloc(sizeof(i32), 1, alignof(i32));
        cstl_assert(num != NULL);
        usize address = (usize)num;
        cstl_assert(address % alignof(i32) == 0);

        *num = 0;

        cstl_assert(*num == 0);

        cstl_assert(num != NULL);
        address = (usize)num;
        cstl_assert(address % alignof(i32) == 0);

        num[0] = 1;
        cstl_assert(num[0] == 1);

        cstl_raw_free((u8*)num, sizeof(i32), 1, alignof(i32));
    }
    {
        const usize len = 10;
        u64* const nums = cstl_alloc(u64, len);
        const usize address = (usize)nums;
        cstl_assert(address % alignof(u64) == 0);

        for (u64 i = 0; i < len; i++) {
            nums[i] = i;
        }
        for (u64 i = 0; i < len; i++) {
            cstl_assert(nums[i] == i);
        }

        cstl_free(u64, nums, len);
    }
    {
        u64* const ptr = cstl_alloc(u64, 0);
        // TODO: this is implementation defined for malloc?
        cstl_assert(ptr != NULL);
        cstl_free(u64, ptr, 0);
    }
}
