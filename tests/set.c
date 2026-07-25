#include "CSTL/set.h"
#include "CSTL/allocator.h"
#include "CSTL/common.h"

typedef const char* string;
u64 string_hash(string key) {
    u64 result = 5381;

    while (*key) {
        result = ((result << 5) + result) + *key;

        key++;
    }

    return result;
}

bool string_equal(string a, string b) {
    return strcmp(a, b) == 0;
}

SET_DEFINE(string)
SET_IMPL(string, string_equal, string_hash)

void test_set(void);

int main(void) {
    test_set();

    return 0;
}

void test_set(void) {
    set_string s = set_string_create(std_allocator());

    cstl_assert(!set_string_contains(s, "ryan"));
    cstl_assert(!set_string_contains(s, "aidan"));
    cstl_assert(!set_string_remove(&s, "ryan"));
    cstl_assert(!set_string_remove(&s, "aidan"));

    set_string_insert(&s, "ryan");
    set_string_insert(&s, "aidan");

    cstl_assert(set_string_contains(s, "ryan"));
    cstl_assert(set_string_contains(s, "aidan"));
    cstl_assert(string_equal(*set_string_remove(&s, "ryan"), "ryan"));
    cstl_assert(string_equal(*set_string_remove(&s, "aidan"), "aidan"));
    cstl_assert(!set_string_contains(s, "ryan"));
    cstl_assert(!set_string_contains(s, "aidan"));
    cstl_assert(!set_string_remove(&s, "ryan"));
    cstl_assert(!set_string_remove(&s, "aidan"));

    set_string_insert(&s, "ryan");
    set_string_insert(&s, "aidan");
    set_string_insert(&s, "joe");
    set_string_insert(&s, "moe");

    set_string_iterator it = set_string_iterator_new(&s);
    cstl_assert(set_string_iterator_next(&it));
    cstl_assert(set_string_iterator_next(&it));
    cstl_assert(set_string_iterator_next(&it));
    cstl_assert(set_string_iterator_next(&it));
    cstl_assert(!set_string_iterator_next(&it));
    cstl_assert(!set_string_iterator_next(&it));

    set_string_free(&s);
    cstl_assert(s.values == NULL);
    cstl_assert(s.states == NULL);
    cstl_assert(s.capacity == 0);
    cstl_assert(s.count == 0);
    cstl_assert(s.tombstone_count == 0);
    cstl_assert(s.alloc.vtable != NULL);
}
