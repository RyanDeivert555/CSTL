#include <stdio.h>
#include "../include/vec.h"
#include "../include/array_hashmap.h"
#include "../include/list.h"
#include "../include/hashmap.h"

#define HASHMAP_BUFF_LEN 1000

VEC_DEFINE(char)
ARRAY_HASHMAP_DEFINE(HASHMAP_BUFF_LEN, int, int)
LIST_DEFINE(float)
HASHMAP_DEFINE(int, float)

void test_vec(void);
void test_array_hashmap(void);
void test_list(void);
void test_hashmap(void);

int main(void) {
    test_vec();
    test_array_hashmap();
    test_list();
    
    return 0;
}

void test_vec(void) {
    printf("vec test start\n");
    vec_char vec1 = vec_char_from_buffer("fdkkn", 5);

    vec_char_set(&vec1, vec_char_get(vec1, 0) + 1, 0);

    for (char* it = iter_char_begin(vec1); it != iter_char_end(vec1); ++it) {
        *it += 1;
    }
    vec_char vec2 = vec_char_from_buffer(" world", sizeof(" world") - 1);
    vec_char_append(&vec1, vec2);
    vec_char_push(&vec1, '\n');

    vec_char_print(vec1);

    vec_char_free(&vec1);
    vec_char_free(&vec2);
    printf("vec test end\n");
}

size_t hash(int key) {
    key ^= key >> 16;
    key *= 0x85ebca6b;
    key ^= key >> 13;
    key *= 0xc2b2ae35;
    key ^= key >> 16;
    
    return (size_t)key;
}

void test_array_hashmap(void) {
    printf("hashmap test start\n");
    array_hashmap_int_to_int map = array_hashmap_int_to_int_new(&hash);
    for (int i = 1; i <= 10; i++) {
        array_hashmap_int_to_int_insert(&map, i * 10, i);
    }
    for (int i = 0; i <= 10; i++) {
        printf("%d ", array_hashmap_int_to_int_get(map, i * 10));
    }
    printf("\n");

    printf("hashmap test end\n");
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

void test_hashmap(void) {

}
