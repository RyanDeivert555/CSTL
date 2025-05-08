#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "../include/vec.h"
#include "../include/list.h"
#include "../include/hashmap.h"
#include "../include/allocator.h"
#include "../include/fba.h"

typedef const char* str;
long long str_hash(str key) {
    long long result = 0;

    while (*key) {
        result = ((result << 5) + result) + *key;

        key++;
    }

    return result;
}

bool str_eql(str a, str b) {
    return a == b;
}

VEC_DEFINE(char)
VEC_IMPL(char)
LIST_DEFINE(float)
LIST_IMPL(float)
HASHMAP_DEFINE(str, int)
HASHMAP_IMPL(str, int, str_hash, str_eql)

void test_vec(void);
void test_list(void);
void test_hashmap(void);
void test_allocator(void);
void test_fba(void);

int main(void) {
    test_vec();
    test_list();
    test_hashmap();
    test_allocator();
    test_fba();

    puts("all tests passed");

    return 0;
}

void test_vec(void) {
    const allocator a = c_allocator();
 
    vec_char vec1 = vec_char_new();
    const char* hello = "gdkkn";
    for (int64_t i = 0; i < 5; i++) {
        vec_char_push(&vec1, a, hello[i]);
    }
 
    for (int64_t i = 0; i < vec1.length; i++) {
        vec1.buffer[i] += 1;
    }
    vec_char_push(&vec1, a, '\n');
 
    const char* expected = "hello\n";
    for (int64_t i = 0; i < vec1.length; i++) {
        assert(expected[i] == vec1.buffer[i]);
    }
 
    vec_char_free(&vec1, a);
 
    puts("vec tests passed\n");
}
 
void test_list(void) {
    const allocator a = c_allocator();
 
    list_float l1 = list_float_new();
    for (float i = 0.0f; i < 10.0f; i++) {
        list_float_push_back(&l1, a, i);
        list_float_push_front(&l1, a, i);
        assert(list_float_pop_back(&l1, a) == i);
    }

    float expected = 9.0f;
    node_float* current = l1.head;    
    while (current) {
        assert(expected == current->data);
        current = current->next;
        expected -= 1.0f;
    }
 
    list_float_free(&l1, a);
 
    puts("list tests passed\n");
}

void test_hashmap(void) {
    const allocator a = c_allocator();

    hashmap_str_int map = hashmap_str_int_new();

    assert(hashmap_str_int_get(&map, "Ryan") == NULL);
    assert(hashmap_str_int_get(&map, "Aidan") == NULL);

    hashmap_str_int_set(&map, a, "Ryan", 19);
    hashmap_str_int_set(&map, a, "Aidan", 16);

    int* v1 = hashmap_str_int_get(&map, "Ryan");
    assert(v1 != NULL);
    assert(*v1 == 19);

    int* v2 = hashmap_str_int_get(&map, "Aidan");
    assert(v2 != NULL);
    assert(*v2 == 16);

    int* v3 = hashmap_str_int_get(&map, "Bob");
    assert(v3 == NULL);

    hashmap_str_int_set(&map, a, "Ryan", 21);
    int* v4 = hashmap_str_int_get(&map, "Ryan");
    assert(v4 != NULL);
    assert(*v4 == 21);

    assert(hashmap_str_int_get(&map, "Bobo") == NULL);
    assert(hashmap_str_int_get(&map, "Momo") == NULL);

    hashmap_str_int_free(&map, a);

    puts("hashmap tests passed\n");
}

void test_allocator(void) {
    const allocator a = c_allocator();

    {
        int* num = (int*)allocator_raw_alloc(a, sizeof(int), 1, _Alignof(int));
        assert(num != NULL);
        uint64_t address = (uint64_t)num;
        assert(address % _Alignof(int) == 0);

        *num = 0;

        assert(*num == 0);

        assert(num != NULL);
        address = (uint64_t)num;
        assert(address % _Alignof(int) == 0);

        num[0] = 1;
        assert(num[0] == 1);

        allocator_raw_free(a, (unsigned char*)num, sizeof(int), 1, _Alignof(int));
    }

    {
        const uint64_t len = 10;
        uint64_t* nums = allocator_alloc(uint64_t, a, len);
        const uint64_t address = (uint64_t)nums;
        assert(address % _Alignof(uint64_t) == 0);

        for (uint64_t i = 0; i < len; i++) {
            nums[i] = i;
        }
        for (uint64_t i = 0; i < len; i++) {
            assert(nums[i] == i);
        }

        allocator_free(uint64_t, a, nums, len);
    }

    puts("allocator test passed\n");
}

void test_fba(void) {
    const int64_t size = 1 << 16;
    uint8_t* buffer = malloc(size);
    fba fba = fba_new(buffer, size);
    const allocator a = fba_as_allocator(&fba);

    {
        int* memory = allocator_alloc(int, a, 1);
        const uint64_t address = (uint64_t)memory;
        assert(address % _Alignof(int) == 0);

        assert(memory != NULL);
        *memory = 10;

        allocator_free(int, a, memory, 1);
    }

    {
        const int64_t length = 10;
        int* memory = allocator_alloc(int, a, length);
    
        assert(memory != NULL);
    }
    free(buffer);

    puts("fba test passed\n");
}

