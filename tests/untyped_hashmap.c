#include "CSTL/untyped_hashmap.h"
#include <string.h>

void test_untyped_hashmap(void);

int main(void) {
    test_untyped_hashmap();

    return 0;
}

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

void test_untyped_hashmap(void) {
    string ryan = "Ryan";
    string aidan = "Aidan";
    string codebase = "Codebase";
    string ringo = "ringo";

    untyped_hashmap map = untyped_hashmap_new(untyped_string_equal, untyped_string_hash);

    cstl_assert(untyped_hashmap_get(&map, sizeof(string), &ryan, sizeof(i32)) == NULL);
    cstl_assert(untyped_hashmap_get(&map, sizeof(string), &aidan, sizeof(i32)) == NULL);

    untyped_hashmap_set(&map, sizeof(string), alignof(string), &ryan, sizeof(i32), alignof(i32), &(i32){20});
    i32* v1 = untyped_hashmap_get(&map, sizeof(string), &ryan, sizeof(i32));
    cstl_assert(v1 != NULL);
    cstl_assert(*v1 == 20);

    untyped_hashmap_set(&map, sizeof(string), alignof(string), &aidan, sizeof(i32), alignof(i32), &(i32){17});
    i32* v2 = untyped_hashmap_get(&map, sizeof(string), &aidan, sizeof(i32));
    cstl_assert(v2 != NULL);
    cstl_assert(*v2 == 17);

    i32* v3 = untyped_hashmap_get(&map, sizeof(string), &codebase, sizeof(i32));
    cstl_assert(v3 == NULL);

    untyped_hashmap_set(&map, sizeof(string), alignof(string), &ryan, sizeof(i32), alignof(i32), &(i32){21});
    i32* v4 = untyped_hashmap_get(&map, sizeof(string), &ryan, sizeof(i32));
    cstl_assert(v4 != NULL);
    cstl_assert(*v4 == 21);

    cstl_assert(untyped_hashmap_get(&map, sizeof(string), &codebase, sizeof(i32)) == NULL);
    cstl_assert(untyped_hashmap_get(&map, sizeof(string), &ringo, sizeof(i32)) == NULL);

    cstl_assert(!untyped_hashmap_try_remove(&map, sizeof(string), &codebase, sizeof(i32), NULL));
    cstl_assert(!untyped_hashmap_try_remove(&map, sizeof(string), &ringo, sizeof(i32), NULL));

    i32 out = 0;
    cstl_assert(untyped_hashmap_try_remove(&map, sizeof(string), &ryan, sizeof(i32), &out));
    cstl_assert(out == 21);

    cstl_assert(untyped_hashmap_get(&map, sizeof(string), &ryan, sizeof(i32)) == NULL);

    untyped_hashmap_set(&map, sizeof(string), alignof(string), &ryan, sizeof(i32), alignof(i32), &(i32){21});
    untyped_hashmap_set(&map, sizeof(string), alignof(string), &codebase, sizeof(i32), alignof(i32), &(i32){39});
    untyped_hashmap_set(&map, sizeof(string), alignof(string), &ringo, sizeof(i32), alignof(i32), &(i32){12});

    untyped_hashmap_iterator it = untyped_hashmap_iterator_new(&map);

    cstl_assert(untyped_hashmap_iterator_next(&it, sizeof(string), sizeof(i32)));
    cstl_assert(untyped_hashmap_iterator_next(&it, sizeof(string), sizeof(i32)));
    cstl_assert(untyped_hashmap_iterator_next(&it, sizeof(string), sizeof(i32)));
    cstl_assert(untyped_hashmap_iterator_next(&it, sizeof(string), sizeof(i32)));
    cstl_assert(!untyped_hashmap_iterator_next(&it, sizeof(string), sizeof(i32)));
    cstl_assert(!untyped_hashmap_iterator_next(&it, sizeof(string), sizeof(i32)));

    untyped_hashmap_free(&map, sizeof(string), alignof(string), sizeof(i32), alignof(i32));
}
