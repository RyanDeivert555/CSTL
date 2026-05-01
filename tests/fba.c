#include "CSTL/fba.h"
#include "CSTL/allocator.h"
#include <stdlib.h>

void test_fba(void);

int main(void) {
    test_fba();

    return 0;
}

void test_fba(void) {
    const i64 buffer_size = 1 << 16;
    u8* const buffer = malloc(buffer_size);
    fba f = fba_new(buffer, buffer_size);
    set_global_allocator(&f, fba_alloc, fba_free);

    {
        i32* const memory = cstl_alloc(i32, 1);
        const usize address = (usize)memory;
        cstl_assert(address % alignof(i32) == 0);

        cstl_assert(memory != NULL);
        *memory = 10;

        cstl_free(i32, memory, 1);
        // free last allocation
        cstl_assert(f.size == 0);
    }
    {
        const usize length = 10;
        i32* const memory = cstl_alloc(i32, length);

        cstl_assert(memory != NULL);
        cstl_free(i32, memory, length);
        cstl_assert(f.size == 0);
    }
    {
        const i32* const first = cstl_alloc(i32, 10);
        const i32* const second = cstl_alloc(i32, 10);
        cstl_free(i32, first, 10);

        // not last allocation, no freeing
        cstl_assert(f.size == 20 * sizeof(i32));
        cstl_free(i32, second, 10);
        cstl_assert(f.size == 10 * sizeof(i32));
        cstl_free(i32, first, 10);
        cstl_assert(f.size == 0);
    }
    {
        u64* const ptr = cstl_alloc(u64, 0);
        cstl_assert(ptr != NULL);
        cstl_free(u64, ptr, 0);
    }

    free(buffer);
}
