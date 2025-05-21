#include "../include/vec.h"
#include "../include/list.h"
#include "../include/hashmap.h"
#include "../include/allocator.h"
#include "../include/fba.h"
#include <stdio.h>
#include <stdlib.h>

typedef const char* str;
i64 str_hash(str key) {
    i64 result = 0;

    while (*key) {
        result = ((result << 5) + result) + *key;

        key++;
    }

    return result;
}

bool str_eql(str a, str b) {
    return a == b;
}

VEC_DEFINE(u8)
VEC_IMPL(u8)
LIST_DEFINE(f32)
LIST_IMPL(f32)
HASHMAP_DEFINE(str, i32)
HASHMAP_IMPL(str, i32, str_hash, str_eql)

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
 
    vec_u8 vec1 = vec_u8_new();
    const char* hello = "gdkkn";
    for (i64 i = 0; i < 5; i++) {
        vec_u8_push(&vec1, a, hello[i]);
    }
 
    for (i64 i = 0; i < vec1.length; i++) {
        vec1.buffer[i] += 1;
    }
    vec_u8_push(&vec1, a, '\n');
 
    const char* expected = "hello\n";
    for (i64 i = 0; i < vec1.length; i++) {
        Assert(expected[i] == vec1.buffer[i]);
    }
 
    vec_u8_free(&vec1, a);
 
    puts("vec tests passed\n");
}
 
void test_list(void) {
    const allocator a = c_allocator();
 
    list_f32 l1 = list_f32_new();
    for (float i = 0.0f; i < 10.0f; i++) {
        list_f32_push_back(&l1, a, i);
        list_f32_push_front(&l1, a, i);
        Assert(list_f32_pop_back(&l1, a) == i);
    }

    float expected = 9.0f;
    node_f32* current = l1.head;    
    while (current) {
        Assert(expected == current->data);
        current = current->next;
        expected -= 1.0f;
    }
 
    list_f32_free(&l1, a);
 
    puts("list tests passed\n");
}

void test_hashmap(void) {
    const allocator a = c_allocator();

    hashmap_str_i32 map = hashmap_str_i32_new();

    Assert(hashmap_str_i32_get(&map, "Ryan") == NULL);
    Assert(hashmap_str_i32_get(&map, "Aidan") == NULL);

    hashmap_str_i32_set(&map, a, "Ryan", 19);
    hashmap_str_i32_set(&map, a, "Aidan", 16);

    i32* v1 = hashmap_str_i32_get(&map, "Ryan");
    Assert(v1 != NULL);
    Assert(*v1 == 19);

    i32* v2 = hashmap_str_i32_get(&map, "Aidan");
    Assert(v2 != NULL);
    Assert(*v2 == 16);

    i32* v3 = hashmap_str_i32_get(&map, "Bob");
    Assert(v3 == NULL);

    hashmap_str_i32_set(&map, a, "Ryan", 21);
    i32* v4 = hashmap_str_i32_get(&map, "Ryan");
    Assert(v4 != NULL);
    Assert(*v4 == 21);

    Assert(hashmap_str_i32_get(&map, "Bobo") == NULL);
    Assert(hashmap_str_i32_get(&map, "Momo") == NULL);

    hashmap_str_i32_free(&map, a);

    puts("hashmap tests passed\n");
}

void test_allocator(void) {
    const allocator a = c_allocator();

    {
        i32* num = (i32*)allocator_raw_alloc(a, sizeof(i32), 1, _Alignof(i32));
        Assert(num != NULL);
        u64 address = (u64)num;
        Assert(address % _Alignof(i32) == 0);

        *num = 0;

        Assert(*num == 0);

        Assert(num != NULL);
        address = (u64)num;
        Assert(address % _Alignof(i32) == 0);

        num[0] = 1;
        Assert(num[0] == 1);

        allocator_raw_free(a, (u8*)num, sizeof(i32), 1, _Alignof(i32));
    }

    {
        const u64 len = 10;
        u64* nums = allocator_alloc(u64, a, len);
        const u64 address = (u64)nums;
        Assert(address % _Alignof(u64) == 0);

        for (u64 i = 0; i < len; i++) {
            nums[i] = i;
        }
        for (u64 i = 0; i < len; i++) {
            Assert(nums[i] == i);
        }

        allocator_free(u64, a, nums, len);
    }

    puts("allocator test passed\n");
}

void test_fba(void) {
    const i64 size = 1 << 16;
    u8* buffer = malloc(size);
    fba fba = fba_new(buffer, size);
    const allocator a = fba_as_allocator(&fba);

    {
        int* memory = allocator_alloc(int, a, 1);
        const u64 address = (u64)memory;
        Assert(address % _Alignof(int) == 0);

        Assert(memory != NULL);
        *memory = 10;

        allocator_free(i32, a, memory, 1);
    }

    {
        const i64 length = 10;
        i32* memory = allocator_alloc(i32, a, length);
    
        Assert(memory != NULL);
    }
    free(buffer);

    puts("fba test passed\n");
}

