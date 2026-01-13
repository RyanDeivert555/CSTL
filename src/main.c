#include "CSTL/allocator.h"
#include "CSTL/common.h"
#include "CSTL/fba.h"
#include "CSTL/hashmap.h"
#include "CSTL/intrusive_list.h"
#include "CSTL/untyped_hashmap.h"
#include "CSTL/untyped_vec.h"
#include "CSTL/vec.h"
#include <stdio.h>
#include <stdlib.h>

void test_vec(void);
void test_hashmap(void);
void test_allocator(void);
void test_fba(void);
void test_untyped_vec(void);
void test_untyped_hashmap(void);
void test_intrusive_list(void);

int main(void) {
    test_vec();
    test_hashmap();
    test_allocator();
    test_fba();
    test_untyped_vec();
    test_untyped_hashmap();
    test_intrusive_list();

    puts("all tests passed");

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

VEC_DEFINE(u8);
VEC_IMPL(u8);
HASHMAP_DEFINE(string, i32);
HASHMAP_IMPL(string, i32, string_equal, string_hash);

void test_vec(void) {
    const allocator a = std_allocator();

    vec_u8 vec = {0};
    const char* hello = "gdkkn";
    for (i64 i = 0; i < 5; i++) {
        vec_u8_push(&vec, a, hello[i]);
    }

    for (i64 i = 0; i < vec.length; i++) {
        vec.buffer[i] += 1;
    }
    vec_u8_push(&vec, a, '\n');

    const char* expected = "hello\n";
    for (i64 i = 0; i < vec.length; i++) {
        cstl_assert(expected[i] == vec.buffer[i]);
    }

    vec_u8_free(&vec, a);

    puts("vec tests passed\n");
}

void test_hashmap(void) {
    const allocator a = std_allocator();

    hashmap_string_i32 map = {0};

    cstl_assert(hashmap_string_i32_get(&map, "Ryan") == NULL);
    cstl_assert(hashmap_string_i32_get(&map, "Aidan") == NULL);

    hashmap_string_i32_set(&map, a, "Ryan", 19);
    hashmap_string_i32_set(&map, a, "Aidan", 16);

    i32* v1 = hashmap_string_i32_get(&map, "Ryan");
    cstl_assert(v1 != NULL);
    cstl_assert(*v1 == 19);

    i32* v2 = hashmap_string_i32_get(&map, "Aidan");
    cstl_assert(v2 != NULL);
    cstl_assert(*v2 == 16);

    i32* v3 = hashmap_string_i32_get(&map, "Bob");
    cstl_assert(v3 == NULL);

    hashmap_string_i32_set(&map, a, "Ryan", 21);
    i32* v4 = hashmap_string_i32_get(&map, "Ryan");
    cstl_assert(v4 != NULL);
    cstl_assert(*v4 == 21);

    cstl_assert(hashmap_string_i32_get(&map, "Bobo") == NULL);
    cstl_assert(hashmap_string_i32_get(&map, "Momo") == NULL);

    cstl_assert(!hashmap_string_i32_try_remove(&map, "Bobo", NULL));
    cstl_assert(!hashmap_string_i32_try_remove(&map, "Momo", NULL));

    i32 out;
    cstl_assert(hashmap_string_i32_try_remove(&map, "Ryan", &out));
    cstl_assert(out == 21);
    cstl_assert(map.count == 1);

    cstl_assert(hashmap_string_i32_get(&map, "Ryan") == NULL);

    hashmap_string_i32_set(&map, a, "Ryan", 21);
    hashmap_string_i32_set(&map, a, "Momo", 39);
    hashmap_string_i32_set(&map, a, "Bobo", 12);
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

    hashmap_string_i32_free(&map, a);

    puts("hashmap tests passed\n");
}

void test_allocator(void) {
    const allocator a = std_allocator();

    {
        i32* num = (i32*)allocator_raw_alloc(a, sizeof(i32), 1, alignof(i32));
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

        allocator_raw_free(a, (u8*)num, sizeof(i32), 1, alignof(i32));
    }
    {
        const usize len = 10;
        u64* nums = allocator_alloc(u64, a, len);
        const usize address = (usize)nums;
        cstl_assert(address % alignof(u64) == 0);

        for (u64 i = 0; i < len; i++) {
            nums[i] = i;
        }
        for (u64 i = 0; i < len; i++) {
            cstl_assert(nums[i] == i);
        }

        allocator_free(u64, a, nums, len);
    }
    {
        u64* ptr = allocator_alloc(u64, a, 0);
        cstl_assert(ptr != NULL);
        allocator_free(u64, a, ptr, 0);
    }

    puts("allocator test passed\n");
}

void test_fba(void) {
    u8 buffer[1 << 16];
    fba f = fba_new(buffer, sizeof(buffer));
    const allocator a = fba_as_allocator(&f);

    {
        i32* memory = allocator_alloc(i32, a, 1);
        const usize address = (usize)memory;
        cstl_assert(address % alignof(i32) == 0);

        cstl_assert(memory != NULL);
        *memory = 10;

        allocator_free(i32, a, memory, 1);
        const fba* instance = a.ctx;
        // free last allocation
        cstl_assert(instance->size == 0);
    }
    {
        const usize length = 10;
        i32* memory = allocator_alloc(i32, a, length);

        cstl_assert(memory != NULL);
        allocator_free(i32, a, memory, length);
        const fba* instance = a.ctx;
        cstl_assert(instance->size == 0);
    }
    {
        const i32* first = allocator_alloc(i32, a, 10);
        const i32* second = allocator_alloc(i32, a, 10);
        allocator_free(i32, a, first, 10);

        const fba* instance = a.ctx;
        // not last allocation, no freeing
        cstl_assert(instance->size == 20 * sizeof(i32));
        allocator_free(i32, a, second, 10);
        cstl_assert(instance->size == 10 * sizeof(i32));
        allocator_free(i32, a, first, 10);
        cstl_assert(instance->size == 0);
    }
    {
        u64* ptr = allocator_alloc(u64, a, 0);
        cstl_assert(ptr != NULL);
        allocator_free(u64, a, ptr, 0);
    }

    puts("fba test passed\n");
}

void test_untyped_vec(void) {
    const allocator a = std_allocator();

    untyped_vec vec = {0};
    const char* hello = "gdkkn";
    for (i64 i = 0; i < 5; i++) {
        untyped_vec_push(&vec, a, sizeof(u8), alignof(u8), &hello[i]);
    }

    for (i64 i = 0; i < vec.length; i++) {
        u8* item = (u8*)untyped_vec_get(&vec, i, sizeof(u8));
        *item += 1;
    }
    untyped_vec_push(&vec, a, sizeof(u8), alignof(u8), &(u8){'\n'});

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

    untyped_vec_free(&vec, a, sizeof(u8), alignof(u8));

    // test with integers now
    memset(&vec, 0, sizeof(untyped_vec));
    cstl_assert(!vec.buffer && !vec.length && !vec.capacity);

    const i32 nums[] = {0, 1, 2, 4, 8, 16, -1};
    for (i64 i = 0; nums[i] != -1; i++) {
        untyped_vec_push(&vec, a, sizeof(i32), alignof(i32), &nums[i]);
    }

    i64 index = 5;
    while (vec.length != 0) {
        const i32* top = untyped_vec_pop(&vec, sizeof(i32));
        cstl_assert(*top == nums[index]);
        index--;
    }

    untyped_vec_free(&vec, a, sizeof(i32), alignof(i32));

    puts("untyped vec tests passed\n");
}

void test_untyped_hashmap(void) {
    const allocator a = std_allocator();

    string ryan = "Ryan";
    string aidan = "Aidan";
    string codebase = "Codebase";
    string ringo = "ringo";

    untyped_hashmap map = untyped_hashmap_new(untyped_string_equal, untyped_string_hash);

    cstl_assert(untyped_hashmap_get(&map, sizeof(string), &ryan, sizeof(i32)) == NULL);
    cstl_assert(untyped_hashmap_get(&map, sizeof(string), &aidan, sizeof(i32)) == NULL);

    untyped_hashmap_set(&map, a, sizeof(string), alignof(string), &ryan, sizeof(i32), alignof(i32), &(i32){20});
    i32* v1 = untyped_hashmap_get(&map, sizeof(string), &ryan, sizeof(i32));
    cstl_assert(v1 != NULL);
    cstl_assert(*v1 == 20);

    untyped_hashmap_set(&map, a, sizeof(string), alignof(string), &aidan, sizeof(i32), alignof(i32), &(i32){17});
    i32* v2 = untyped_hashmap_get(&map, sizeof(string), &aidan, sizeof(i32));
    cstl_assert(v2 != NULL);
    cstl_assert(*v2 == 17);

    i32* v3 = untyped_hashmap_get(&map, sizeof(string), &codebase, sizeof(i32));
    cstl_assert(v3 == NULL);

    untyped_hashmap_set(&map, a, sizeof(string), alignof(string), &ryan, sizeof(i32), alignof(i32), &(i32){21});
    i32* v4 = untyped_hashmap_get(&map, sizeof(string), &ryan, sizeof(i32));
    cstl_assert(v4 != NULL);
    cstl_assert(*v4 == 21);

    cstl_assert(untyped_hashmap_get(&map, sizeof(string), &codebase, sizeof(i32)) == NULL);
    cstl_assert(untyped_hashmap_get(&map, sizeof(string), &ringo, sizeof(i32)) == NULL);

    cstl_assert(!untyped_hashmap_try_remove(&map, sizeof(string), &codebase, sizeof(i32), NULL));
    cstl_assert(!untyped_hashmap_try_remove(&map, sizeof(string), &ringo, sizeof(i32), NULL));

    void* out = NULL;
    cstl_assert(untyped_hashmap_try_remove(&map, sizeof(string), &ryan, sizeof(i32), &out));
    cstl_assert(*(i32*)out == 21);

    cstl_assert(untyped_hashmap_get(&map, sizeof(string), &ryan, sizeof(i32)) == NULL);

    untyped_hashmap_set(&map, a, sizeof(string), alignof(string), &ryan, sizeof(i32), alignof(i32), &(i32){21});
    untyped_hashmap_set(&map, a, sizeof(string), alignof(string), &codebase, sizeof(i32), alignof(i32), &(i32){39});
    untyped_hashmap_set(&map, a, sizeof(string), alignof(string), &ringo, sizeof(i32), alignof(i32), &(i32){12});

    untyped_hashmap_iterator it = untyped_hashmap_iterator_new(&map);

    cstl_assert(untyped_hashmap_iterator_next(&it, sizeof(string), sizeof(i32)));
    cstl_assert(untyped_hashmap_iterator_next(&it, sizeof(string), sizeof(i32)));
    cstl_assert(untyped_hashmap_iterator_next(&it, sizeof(string), sizeof(i32)));
    cstl_assert(untyped_hashmap_iterator_next(&it, sizeof(string), sizeof(i32)));
    cstl_assert(!untyped_hashmap_iterator_next(&it, sizeof(string), sizeof(i32)));
    cstl_assert(!untyped_hashmap_iterator_next(&it, sizeof(string), sizeof(i32)));

    untyped_hashmap_free(&map, a, sizeof(string), alignof(string), sizeof(i32), alignof(i32));

    puts("untyped hashmap tests passed\n");
}

void test_intrusive_list(void) {
    typedef struct data {
        i32 num;
        intrusive_node node;
    } data;

    intrusive_list list = {0};
    data one = {1, {0}};
    data two = {2, {0}};
    data three = {3, {0}};
    data four = {4, {0}};
    data five = {5, {0}};
    intrusive_list_push_front(&list, &two.node);
    intrusive_node_insert_after(&two.node, &five.node);
    intrusive_list_push_front(&list, &one.node);
    intrusive_node_insert_after(&two.node, &three.node);
    intrusive_node_insert_after(&three.node, &four.node);

    {
        intrusive_node* curr = list.head;
        i64 count = 0;
        while (curr) {
            count++;
            curr = curr->next;
        }
        cstl_assert(count == 5);
    }
    {
        intrusive_node* curr = list.head;
        i32 index = 1;
        while (curr) {
            const data* d = parent_of(data, node, curr);
            cstl_assert(d->num == index);
            index++;
            curr = curr->next;
        }
    }

    puts("intrusive list tests passed\n");
}
