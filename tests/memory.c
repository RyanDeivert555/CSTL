#include "CSTL/memory.h"

void test_memory(void);

int main(void) {
    test_memory();

    return 0;
}

void test_memory(void) {
    {
        const char* hello = "hello";
        char copy[sizeof("hello")];
        cstl_memcpy(char, copy, hello, 6);

        cstl_assert(cstl_memcmp(char, hello, copy, 6));
    }
    {
        const i32 a = -1;
        const u32 b = cstl_bitcast(u32, a);
        cstl_assert(b == U32_MAX);
    }
    {
        const u16 v = 1 | (1 << 8);
        u8 out[2];
        cstl_array_bitcast(u8[2], out, v);

        cstl_assert(out[0] == 1 && out[1] == 1);
    }
}
