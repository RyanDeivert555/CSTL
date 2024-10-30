#include <assert.h>
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

int str_eql(str a, str b) {
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

    printf("all tests passed");
    
    return 0;
}

void test_vec(void) {
    const allocator a = c_allocator();
 
    vec_char vec1 = vec_char_new();
    const char* hello = "gdkkn";
    for (size_t i = 0; i < 5; i++) {
        vec_char_push(a, &vec1, hello[i]);
    }
 
    for (size_t i = 0; i < vec1.length; i++) {
        vec1.ptr[i] += 1;
    }
    vec_char vec2 = vec_char_new();
    const char* world = " world";
    for (size_t i = 0; i < 6; i++) {
        vec_char_push(a, &vec2, world[i]);
    }
 
    vec_char_append(a, &vec1, vec2);
    vec_char_push(a, &vec1, '\n');
 
    const char* expected = "hello world\n";
    for (size_t i = 0; i < vec1.length; i++) {
        assert(expected[i] == vec1.ptr[i]);
    }
 
    vec_char_free(a, &vec1);
    vec_char_free(a, &vec2);
 
    printf("vec tests passed\n");
}
 
void test_list(void) {
    const allocator a = c_allocator();
 
    list_float l1 = list_float_new();
    for (float i = 0.0f; i < 10.0f; i++) {
        list_float_push_back(a, &l1, i);
        list_float_push_front(a, &l1, i);
        assert(list_float_pop_back(a, &l1) == i);
    }

    float expected = 9.0f;
    node_float* current = l1.head;    
    while (current) {
        assert(expected == current->data);
        current = current->next;
        expected -= 1.0f;
    }
 
    list_float_free(a, &l1);
 
    printf("list tests passed\n");
}

void test_hashmap(void) {
    const allocator a = c_allocator();

    hashmap_str_int map = hashmap_str_int_new();

    hashmap_str_int_set(a, &map, "Ryan", 19);
    hashmap_str_int_set(a, &map, "Aidan", 16);

    int* v1 = hashmap_str_int_get(&map, "Ryan");
    assert(v1 != NULL);
    assert(*v1 == 19);

    int* v2 = hashmap_str_int_get(&map, "Aidan");
    assert(v2 != NULL);
    assert(*v2 == 16);

    int* v3 = hashmap_str_int_get(&map, "Bob");
    assert(v3 == NULL);

    hashmap_str_int_set(a, &map, "Ryan", 21);
    int* v4 = hashmap_str_int_get(&map, "Ryan");
    assert(v4 != NULL);
    assert(*v4 == 21);

    hashmap_str_int_free(a, &map);
    printf("hashmap tests passed\n");
}

void test_allocator(void) {
    const allocator a = c_allocator();

    int* num = (int*)allocator_raw_alloc(a, sizeof(int), _Alignof(int));
    assert(num != NULL);
    size_t address = (size_t)num;
    assert(address % _Alignof(int) == 0);

    *num = 0;

    assert(*num == 0);

    assert(allocator_raw_realloc(a, (unsigned char**)&num, sizeof(int), sizeof(int) * 2, _Alignof(int)));

    assert(num != NULL);
    address = (size_t)num;
    assert(address % _Alignof(int) == 0);

    num[0] = 1;
    num[1] = 1;

    assert(num[0] = 1);
    assert(num[1] = 1);

    allocator_raw_free(a, (unsigned char*)num, sizeof(int), _Alignof(int));

    size_t len = 10;
    size_t* nums = allocator_alloc(size_t, a, len);
    address = (size_t)nums;
    assert(address % _Alignof(size_t) == 0);

    for (size_t i = 0; i < len; i++) {
        nums[i] = i;
    }
    for (size_t i = 0; i < len; i++) {
        assert(nums[i] == i);
    }

    size_t new_len = 100;

    assert(allocator_realloc(size_t, a, &nums, len, new_len));

    allocator_free(size_t, a, nums, len);

    printf("allocator test passed\n");
}

void test_fba(void) {
    const size_t size = 1000000;
    unsigned char* buffer = malloc(size);
    fba fba = fba_new(buffer, size);
    const allocator a = fba_as_allocator(&fba);

    int* memory = allocator_create(int, a);
    size_t address = (size_t)memory;
    assert(address % _Alignof(int) == 0);

    assert(memory != NULL);
    *memory = 10;

    size_t* more_memory = allocator_create(size_t, a);
    address = (size_t)more_memory;
    assert(address % _Alignof(size_t) == 0);

    assert(more_memory != NULL);
    assert(*memory == 10);

    size_t new_len = 10;

    assert(allocator_realloc(size_t, a, &more_memory, 1, new_len));
    // not last alloc, will not reallocate
    assert(!allocator_realloc(int, a, &memory, 1, 100));

    allocator_destroy(int, a, memory);
    allocator_free(size_t, a, more_memory, new_len);
    free(buffer);

    printf("fba test passed\n");
}

