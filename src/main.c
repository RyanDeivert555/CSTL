#include "common.h"
#include "vec.h"
#include "list.h"
#include "hashmap.h"
#include "allocator.h"
#include "fba.h"
#include "untyped_vec.h"
#include <stdio.h>
#include <stdlib.h>

typedef const char* String;
i64 StringHash(String key) {
    i64 result = 0;

    while (*key) {
        result = ((result << 5) + result) + *key;

        key++;
    }

    return result;
}

bool StringEqual(String a, String b) {
    return a == b;
}

VEC_DEFINE(u8)
VEC_IMPL(u8)
LIST_DEFINE(f32)
LIST_IMPL(f32)
HASHMAP_DEFINE(String, i32)
HASHMAP_IMPL(String, i32, StringHash, StringEqual)

void TestVec(void);
void TestList(void);
void TestHashmap(void);
void TestAllocator(void);
void TestFba(void);
void TestUntypedVec(void);

int main(void) {
    TestVec();
    TestList();
    TestHashmap();
    TestAllocator();
    TestFba();
    TestUntypedVec();

    puts("all tests passed");

    return 0;
}

void TestVec(void) {
    const Allocator a = StdAllocator();
 
    Vec_u8 vec = Vec_u8_New();
    const char* hello = "gdkkn";
    for (i64 i = 0; i < 5; i++) {
        Vec_u8_Push(&vec, a, hello[i]);
    }
 
    for (i64 i = 0; i < vec.length; i++) {
        vec.buffer[i] += 1;
    }
    Vec_u8_Push(&vec, a, '\n');
 
    const char* expected = "hello\n";
    for (i64 i = 0; i < vec.length; i++) {
        Assert(expected[i] == vec.buffer[i]);
    }
 
    Vec_u8_Free(&vec, a);
 
    puts("vec tests passed\n");
}
 
void TestList(void) {
    const Allocator a = StdAllocator();
 
    List_f32 list = List_f32_New();
    for (float i = 0.0f; i < 10.0f; i++) {
        List_f32_PushBack(&list, a, i);
        List_f32_PushFront(&list, a, i);
        Assert(List_f32_PopBack(&list, a) == i);
    }

    float expected = 9.0f;
    Node_f32* current = list.head;
    while (current) {
        Assert(expected == current->data);
        current = current->next;
        expected -= 1.0f;
    }
 
    List_f32_Free(&list, a);
 
    puts("list tests passed\n");
}

void TestHashmap(void) {
    const Allocator a = StdAllocator();

    Hashmap_String_i32 map = Hashmap_String_i32_New();

    Assert(Hashmap_String_i32_Get(&map, "Ryan") == NULL);
    Assert(Hashmap_String_i32_Get(&map, "Aidan") == NULL);

    Hashmap_String_i32_Set(&map, a, "Ryan", 19);
    Hashmap_String_i32_Set(&map, a, "Aidan", 16);

    i32* v1 = Hashmap_String_i32_Get(&map, "Ryan");
    Assert(v1 != NULL);
    Assert(*v1 == 19);

    i32* v2 = Hashmap_String_i32_Get(&map, "Aidan");
    Assert(v2 != NULL);
    Assert(*v2 == 16);

    i32* v3 = Hashmap_String_i32_Get(&map, "Bob");
    Assert(v3 == NULL);

    Hashmap_String_i32_Set(&map, a, "Ryan", 21);
    i32* v4 = Hashmap_String_i32_Get(&map, "Ryan");
    Assert(v4 != NULL);
    Assert(*v4 == 21);

    Assert(Hashmap_String_i32_Get(&map, "Bobo") == NULL);
    Assert(Hashmap_String_i32_Get(&map, "Momo") == NULL);

    Hashmap_String_i32_Free(&map, a);

    puts("hashmap tests passed\n");
}

void TestAllocator(void) {
    const Allocator a = StdAllocator();

    {
        i32* num = (i32*)AllocatorRawAlloc(a, sizeof(i32), 1, _Alignof(i32));
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

        AllocatorRawFree(a, (u8*)num, sizeof(i32), 1, _Alignof(i32));
    }

    {
        const u64 len = 10;
        u64* nums = AllocatorAlloc(u64, a, len);
        const u64 address = (u64)nums;
        Assert(address % _Alignof(u64) == 0);

        for (u64 i = 0; i < len; i++) {
            nums[i] = i;
        }
        for (u64 i = 0; i < len; i++) {
            Assert(nums[i] == i);
        }

        AllocatorFree(u64, a, nums, len);
    }

    puts("allocator test passed\n");
}

void TestFba(void) {
    const i64 size = 1 << 16;
    u8* buffer = malloc(size);
    Fba fba = FbaNew(buffer, size);
    const Allocator a = FbaAsAllocator(&fba);

    {
        int* memory = AllocatorAlloc(int, a, 1);
        const u64 address = (u64)memory;
        Assert(address % _Alignof(int) == 0);

        Assert(memory != NULL);
        *memory = 10;

        AllocatorFree(i32, a, memory, 1);
        const Fba* instance = a.ctx;
        // Free last allocation
        Assert(instance->size == 0);
    }

    {
        const i64 length = 10;
        i32* memory = AllocatorAlloc(i32, a, length);
    
        Assert(memory != NULL);
        AllocatorFree(i32, a, memory, length);
        const Fba* instance = a.ctx;
        // Free last allocation
        Assert(instance->size == 0);
    }
    {
        const i32* first = AllocatorAlloc(i32, a, 10);
        const i32* second = AllocatorAlloc(i32, a, 10);
        AllocatorFree(i32, a, first, 10);

        const Fba* instance = a.ctx;
        // not last allocation, no freeing
        Assert(instance->size == 20 * sizeof(i32));
        AllocatorFree(i32, a, second, 10);
        Assert(instance->size == 10 * sizeof(i32));
        AllocatorFree(i32, a, first, 10);
        Assert(instance->size == 0);
    }
    free(buffer);

    puts("fba test passed\n");
}

void TestUntypedVec(void) {
    const Allocator a = StdAllocator();

    UntypedVec vec = UntypedVecNew();
    const char* hello = "gdkkn";
    for (i64 i = 0; i < 5; i++) {
        UntypedVecPush(&vec, a, sizeof(u8), _Alignof(u8), &hello[i]);
    }
 
    for (i64 i = 0; i < vec.length; i++) {
        u8* item = (u8*)UntypedVecGet(&vec, i, sizeof(u8));
        *item += 1;
    }
    UntypedVecPush(&vec, a, sizeof(u8), _Alignof(u8), &(u8){'\n'});
 
    const char* expected = "hello\n";
    for (i64 i = 0; i < vec.length; i++) {
        const u8 c = *(u8*)UntypedVecGet(&vec, i, sizeof(u8));
        Assert(expected[i] == c);
    }

    UntypedVecRemove(&vec, vec.length - 1, sizeof(u8));

    const u8* last = UntypedVecPop(&vec, sizeof(u8));
    Assert(*last == 'o');

    const u8* first = UntypedVecGet(&vec, 0, sizeof(u8));
    Assert(*first == 'h');
 
    UntypedVecFree(&vec, a, sizeof(u8), _Alignof(u8));

    // test with integers now
    vec = UntypedVecNew();
    Assert(!vec.buffer && !vec.length && !vec.capacity);

    const i32 nums[] = {0, 1, 2, 4, 8, 16, -1};
    for (i64 i = 0; nums[i] != -1; i++) {
        UntypedVecPush(&vec, a, sizeof(i32), _Alignof(i32), &nums[i]);
    }

    i64 index = 5;
    while (vec.length != 0) {
        const i32* top = UntypedVecPop(&vec, sizeof(i32));
        Assert(*top == nums[index]);
        index--;
    }

    UntypedVecFree(&vec, a, sizeof(i32), _Alignof(i32));
 
    puts("untyped vec tests passed\n");
}

