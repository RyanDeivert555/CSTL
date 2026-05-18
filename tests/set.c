#include "CSTL/set.h"
#include "CSTL/common.h"

typedef const char* string;
i64 string_hash(string key) {
    i64 result = 5381;

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
    set_string s = {0};

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
}
