#include "CSTL/untyped_set.h"
#include "CSTL/common.h"
#include <string.h>

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

i64 untyped_string_hash(const void* key) {
    return string_hash(*(string*)key);
}

bool untyped_string_equal(const void* a, const void* b) {
    return string_equal(*(string*)a, *(string*)b);
}

void untyped_set_test(void);

int main(void) {
    untyped_set_test();

    return 0;
}

void untyped_set_test(void) {
    untyped_set s = untyped_set_new(untyped_string_equal, untyped_string_hash);

    string ryan = "ryan";
    string aidan = "aidan";
    string joe = "joe";
    string moe = "moe";

    cstl_assert(!untyped_set_contains(s, sizeof(string), &ryan));
    cstl_assert(!untyped_set_contains(s, sizeof(string), &aidan));
    cstl_assert(!untyped_set_remove(&s, sizeof(string), &ryan));
    cstl_assert(!untyped_set_remove(&s, sizeof(string), &aidan));

    untyped_set_insert(&s, sizeof(string), alignof(string), &ryan);
    untyped_set_insert(&s, sizeof(string), alignof(string), &aidan);

    cstl_assert(untyped_set_contains(s, sizeof(string), &ryan));
    cstl_assert(untyped_set_contains(s, sizeof(string), &aidan));
    cstl_assert(string_equal(*(string*)untyped_set_remove(&s, sizeof(string), &ryan), "ryan"));
    cstl_assert(string_equal(*(string*)untyped_set_remove(&s, sizeof(string), &aidan), "aidan"));
    cstl_assert(!untyped_set_contains(s, sizeof(string), &ryan));
    cstl_assert(!untyped_set_contains(s, sizeof(string), &aidan));
    cstl_assert(!untyped_set_remove(&s, sizeof(string), &ryan));
    cstl_assert(!untyped_set_remove(&s, sizeof(string), &aidan));

    untyped_set_insert(&s, sizeof(string), alignof(string), &ryan);
    untyped_set_insert(&s, sizeof(string), alignof(string), &aidan);
    untyped_set_insert(&s, sizeof(string), alignof(string), &joe);
    untyped_set_insert(&s, sizeof(string), alignof(string), &moe);

    untyped_set_iterator it = untyped_set_iterator_new(&s);
    cstl_assert(untyped_set_iterator_next(&it, sizeof(string)));
    cstl_assert(untyped_set_iterator_next(&it, sizeof(string)));
    cstl_assert(untyped_set_iterator_next(&it, sizeof(string)));
    cstl_assert(untyped_set_iterator_next(&it, sizeof(string)));
    cstl_assert(!untyped_set_iterator_next(&it, sizeof(string)));
    cstl_assert(!untyped_set_iterator_next(&it, sizeof(string)));

    untyped_set_free(&s, sizeof(string), alignof(string));
}
