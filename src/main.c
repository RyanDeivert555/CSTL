#include <stdio.h>
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
    printf("vec test start\n");
    vec_char vec1 = vec_char_from_buffer("fdkkn", 5);

    vec_char_set(&vec1, vec_char_get(vec1, 0) + 1, 0);

    for (size_t i = 0; i < vec1.length; i++) {
        vec1.ptr[i] += 1;
    }
    vec_char vec2 = vec_char_from_buffer(" world", sizeof(" world") - 1);
    vec_char_append(&vec1, vec2);
    vec_char_push(&vec1, '\n');

    fwrite(vec1.ptr, sizeof(char), vec1.length, stdout);

    vec_char_free(&vec1);
    vec_char_free(&vec2);
    printf("vec test end\n");
}

void test_list(void) {
    printf("list test start\n");
    list_float l1 = list_float_new();
    for (float i = 0.0f; i < 10.0f; i++) {
        list_float_push_back(&l1, i);
        list_float_push_front(&l1, i);
        list_float_pop_back(&l1);
    }

    node_float* current = l1.head;
    while (current) {
        printf("%f ", current->data);
        current = current->next;
    }
    printf("\n");
    list_float_free(&l1);
    printf("list test end\n");
}
