#include "CSTL/hashmap.h"

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

HASHMAP_DEFINE(string, i32)
HASHMAP_IMPL(string, i32, string_equal, string_hash)

void test_hashmap(void);

int main(void) {
    test_hashmap();

    return 0;
}

void test_hashmap(void) {
    hashmap_string_i32 map = {0};

    cstl_assert(hashmap_string_i32_get(&map, "Ryan") == NULL);
    cstl_assert(hashmap_string_i32_get(&map, "Aidan") == NULL);

    hashmap_string_i32_set(&map, "Ryan", 19);
    hashmap_string_i32_set(&map, "Aidan", 16);

    i32* v1 = hashmap_string_i32_get(&map, "Ryan");
    cstl_assert(v1 != NULL);
    cstl_assert(*v1 == 19);

    i32* v2 = hashmap_string_i32_get(&map, "Aidan");
    cstl_assert(v2 != NULL);
    cstl_assert(*v2 == 16);

    i32* v3 = hashmap_string_i32_get(&map, "Bob");
    cstl_assert(v3 == NULL);

    hashmap_string_i32_set(&map, "Ryan", 21);
    i32* v4 = hashmap_string_i32_get(&map, "Ryan");
    cstl_assert(v4 != NULL);
    cstl_assert(*v4 == 21);

    cstl_assert(hashmap_string_i32_get(&map, "Bobo") == NULL);
    cstl_assert(hashmap_string_i32_get(&map, "Momo") == NULL);

    cstl_assert(!hashmap_string_i32_try_remove(&map, "Bobo", NULL));
    cstl_assert(!hashmap_string_i32_try_remove(&map, "Momo", NULL));

    i32 out = 0;
    cstl_assert(hashmap_string_i32_try_remove(&map, "Ryan", &out));
    cstl_assert(out == 21);
    cstl_assert(map.count == 1);

    cstl_assert(hashmap_string_i32_get(&map, "Ryan") == NULL);

    hashmap_string_i32_set(&map, "Ryan", 21);
    hashmap_string_i32_set(&map, "Momo", 39);
    hashmap_string_i32_set(&map, "Bobo", 12);
    cstl_assert(map.count == 4);

    hashmap_string_i32_iterator it = hashmap_string_i32_iterator_new(&map);

    cstl_assert(hashmap_string_i32_iterator_next(&it));
    cstl_assert(hashmap_string_i32_iterator_next(&it));
    cstl_assert(hashmap_string_i32_iterator_next(&it));
    cstl_assert(hashmap_string_i32_iterator_next(&it));
    cstl_assert(!hashmap_string_i32_iterator_next(&it));
    cstl_assert(!hashmap_string_i32_iterator_next(&it));

    cstl_assert(hashmap_string_i32_try_remove(&map, "Ryan", &out));
    cstl_assert(out == 21);
    cstl_assert(hashmap_string_i32_try_remove(&map, "Aidan", &out));
    cstl_assert(out == 16);
    cstl_assert(hashmap_string_i32_try_remove(&map, "Momo", &out));
    cstl_assert(out == 39);
    cstl_assert(hashmap_string_i32_try_remove(&map, "Bobo", &out));
    cstl_assert(out == 12);
    cstl_assert(map.count == 0);
    cstl_assert(!hashmap_string_i32_try_remove(&map, "Bobo", NULL));
    cstl_assert(map.count == 0);

    hashmap_string_i32_free(&map);
}
