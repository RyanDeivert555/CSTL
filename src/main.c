#include <assert.h>
#include <stdio.h>
#include "../include/vec.h"
#include "../include/list.h"
#include "../include/allocator.h"

VEC_DEFINE(char)
VEC_IMPLEMENT(char)
LIST_DEFINE(float)
LIST_IMPLEMENT(float)

void test_vec(void);
void test_list(void);
void test_allocator(void);

int main(void) {
    test_vec();
    test_list();
    test_allocator();

    printf("all tests passed");
    
    return 0;
}

void test_vec(void) {
    allocator allocator = c_allocator();
 
    vec_char vec1 = vec_char_new(allocator);
    const char* hello = "gdkkn";
    for (size_t i = 0; i < 5; i++) {
        vec_char_push(&vec1, hello[i]);
    }
 
    for (size_t i = 0; i < vec1.length; i++) {
        vec1.ptr[i] += 1;
    }
    vec_char vec2 = vec_char_new(allocator);
    const char* world = " world";
    for (size_t i = 0; i < 6; i++) {
        vec_char_push(&vec2, world[i]);
    }
 
    vec_char_append(&vec1, vec2);
    vec_char_push(&vec1, '\n');
 
    const char* expected = "hello world\n";
    for (size_t i = 0; i < vec1.length; i++) {
        assert(expected[i] == vec1.ptr[i]);
    }
 
    vec_char_free(&vec1);
    vec_char_free(&vec2);
 
    printf("vec tests passed\n");
}
 
void test_list(void) {
    allocator allocator = c_allocator();
 
    list_float l1 = list_float_new(allocator);
    for (float i = 0.0f; i < 10.0f; i++) {
        list_float_push_back(&l1, i);
        list_float_push_front(&l1, i);
        assert(list_float_pop_back(&l1) == i);
    }

    float expected = 9.0f;
    node_float* current = l1.head;    
    while (current) {
        assert(expected == current->data);
        current = current->next;
        expected -= 1.0f;
    }
 
    list_float_free(&l1);
 
    printf("list tests passed\n");
}

void test_allocator(void) {
    allocator a = c_allocator();

    int* num = (int*)allocator_raw_alloc(a, sizeof(int));

    assert(num != NULL);

    *num = 0;

    assert(*num == 0);

    assert(allocator_raw_realloc(a, (unsigned char**)&num, sizeof(int), sizeof(int) * 2));

    assert(num != NULL);

    num[0] = 1;
    num[1] = 1;

    assert(num[0] = 1);
    assert(num[1] = 1);

    allocator_raw_free(a, (unsigned char*)num, 1);

    size_t len = 10;
    size_t* nums = allocator_alloc(size_t, a, len);

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
