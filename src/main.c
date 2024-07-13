#include <assert.h>
#include "../include/vec.h"
#include "../include/list.h"

VEC_DEFINE(char)
VEC_IMPLEMENT(char)
LIST_DEFINE(float)
LIST_IMPLEMENT(float)

void test_vec(void);
void test_list(void);

int main(void) {
    test_vec();
    test_list();
    
    return 0;
}

void test_vec(void) {
    vec_char vec1 = vec_char_new();
    const char* hello = "gdkkn";
    for (size_t i = 0; i < 5; i++) {
        vec_char_push(&vec1, hello[i]);
    }

    for (size_t i = 0; i < vec1.length; i++) {
        vec1.ptr[i] += 1;
    }
    vec_char vec2 = vec_char_new();
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
}

void test_list(void) {
    list_float l1 = list_float_new();
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
}
