#include "common.h"
#include "vec.h"
#include "hashmap.h"
#include "allocator.h"
#include "fba.h"
#include "untyped_vec.h"
#include "untyped_hashmap.h"
#include "intrusive_list.h"
#include <stdio.h>
#include <stdlib.h>

void TestVec(void);
void TestHashmap(void);
void TestAllocator(void);
void TestFba(void);
void TestUntypedVec(void);
void TestUntypedHashmap(void);
void TestIntrusiveList(void);

int main(void) {
    TestVec();
    TestHashmap();
    TestAllocator();
    TestFba();
    TestUntypedVec();
    TestUntypedHashmap();
    TestIntrusiveList();

    puts("all tests passed");

    return 0;
}

typedef const char* String;
i64 StringHash(String key) {
    i64 result = 5381;

    while (*key) {
        result = ((result << 5) + result) + *key;

        key++;
    }

    return result;
}

bool StringEqual(String a, String b) {
    return strcmp(a, b) == 0;
}

i64 UntypedStringHash(const void* key) {
    return StringHash(*(String*)key);
}

bool UntypedStringEqual(const void* a, const void* b) {
    return StringEqual(*(String*)a, *(String*)b);
}

VEC_DEFINE(u8);
VEC_IMPL(u8);
HASHMAP_DEFINE(String, i32);
HASHMAP_IMPL(String, i32, StringEqual, StringHash);

void TestVec(void) {
    const Allocator a = StdAllocator();
 
    Vec_u8 vec = {0};
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
 
void TestHashmap(void) {
    const Allocator a = StdAllocator();

    Hashmap_String_i32 map = {0};

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

    Assert(!Hashmap_String_i32_TryRemove(&map, "Bobo", NULL));
    Assert(!Hashmap_String_i32_TryRemove(&map, "Momo", NULL));

    i32 out;
    Assert(Hashmap_String_i32_TryRemove(&map, "Ryan", &out));
    Assert(out == 21);
    Assert(map.count == 1);

    Assert(Hashmap_String_i32_Get(&map, "Ryan") == NULL);

    Hashmap_String_i32_Set(&map, a, "Ryan", 21);
    Hashmap_String_i32_Set(&map, a, "Momo", 39);
    Hashmap_String_i32_Set(&map, a, "Bobo", 12);
    Assert(map.count == 4);

    Hashmap_String_i32_Iterator it = Hashmap_String_i32_IteratorNew(&map);

    Assert(Hashmap_String_i32_IteratorNext(&it));
    Assert(Hashmap_String_i32_IteratorNext(&it));
    Assert(Hashmap_String_i32_IteratorNext(&it));
    Assert(Hashmap_String_i32_IteratorNext(&it));
    Assert(!Hashmap_String_i32_IteratorNext(&it));
    Assert(!Hashmap_String_i32_IteratorNext(&it));

    Assert(Hashmap_String_i32_TryRemove(&map, "Ryan", &out));
    Assert(out == 21);
    Assert(Hashmap_String_i32_TryRemove(&map, "Aidan", &out));
    Assert(out == 16);
    Assert(Hashmap_String_i32_TryRemove(&map, "Momo", &out));
    Assert(out == 39);
    Assert(Hashmap_String_i32_TryRemove(&map, "Bobo", &out));
    Assert(out == 12);
    Assert(map.count == 0);
    Assert(!Hashmap_String_i32_TryRemove(&map, "Bobo", NULL));
    Assert(map.count == 0);

    Hashmap_String_i32_Free(&map, a);

    puts("hashmap tests passed\n");
}

void TestAllocator(void) {
    const Allocator a = StdAllocator();

    {
        i32* num = (i32*)AllocatorRawAlloc(a, sizeof(i32), 1, alignof(i32));
        Assert(num != NULL);
        usize address = (usize)num;
        Assert(address % alignof(i32) == 0);

        *num = 0;

        Assert(*num == 0);

        Assert(num != NULL);
        address = (usize)num;
        Assert(address % alignof(i32) == 0);

        num[0] = 1;
        Assert(num[0] == 1);

        AllocatorRawFree(a, (u8*)num, sizeof(i32), 1, alignof(i32));
    }
    {
        const usize len = 10;
        u64* nums = AllocatorAlloc(u64, a, len);
        const usize address = (usize)nums;
        Assert(address % alignof(u64) == 0);

        for (u64 i = 0; i < len; i++) {
            nums[i] = i;
        }
        for (u64 i = 0; i < len; i++) {
            Assert(nums[i] == i);
        }

        AllocatorFree(u64, a, nums, len);
    }
    {
        u64* ptr = AllocatorAlloc(u64, a, 0);
        Assert(ptr != NULL);
        AllocatorFree(u64, a, ptr, 0);
    }

    puts("allocator test passed\n");
}

void TestFba(void) {
    u8 buffer[1 << 16];
    Fba fba = FbaNew(buffer, sizeof(buffer));
    const Allocator a = FbaAsAllocator(&fba);

    {
        i32* memory = AllocatorAlloc(i32, a, 1);
        const usize address = (usize)memory;
        Assert(address % alignof(i32) == 0);

        Assert(memory != NULL);
        *memory = 10;

        AllocatorFree(i32, a, memory, 1);
        const Fba* instance = a.ctx;
        // Free last allocation
        Assert(instance->size == 0);
    }
    {
        const usize length = 10;
        i32* memory = AllocatorAlloc(i32, a, length);
    
        Assert(memory != NULL);
        AllocatorFree(i32, a, memory, length);
        const Fba* instance = a.ctx;
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
    {
        u64* ptr = AllocatorAlloc(u64, a, 0);
        Assert(ptr != NULL);
        AllocatorFree(u64, a, ptr, 0);
    }

    puts("fba test passed\n");
}

void TestUntypedVec(void) {
    const Allocator a = StdAllocator();

    UntypedVec vec = {0};
    const char* hello = "gdkkn";
    for (i64 i = 0; i < 5; i++) {
        UntypedVecPush(&vec, a, sizeof(u8), alignof(u8), &hello[i]);
    }
 
    for (i64 i = 0; i < vec.length; i++) {
        u8* item = (u8*)UntypedVecGet(&vec, i, sizeof(u8));
        *item += 1;
    }
    UntypedVecPush(&vec, a, sizeof(u8), alignof(u8), &(u8){'\n'});
 
    const u8* expected = (const u8*)"hello\n";
    for (i64 i = 0; i < vec.length; i++) {
        const u8 c = *(u8*)UntypedVecGet(&vec, i, sizeof(u8));
        Assert(expected[i] == c);
    }

    UntypedVecRemove(&vec, vec.length - 1, sizeof(u8));

    const u8* last = UntypedVecPop(&vec, sizeof(u8));
    Assert(*last == 'o');

    const u8* first = UntypedVecGet(&vec, 0, sizeof(u8));
    Assert(*first == 'h');
 
    UntypedVecFree(&vec, a, sizeof(u8), alignof(u8));

    // test with integers now
    memset(&vec, 0, sizeof(UntypedVec));
    Assert(!vec.buffer && !vec.length && !vec.capacity);

    const i32 nums[] = {0, 1, 2, 4, 8, 16, -1};
    for (i64 i = 0; nums[i] != -1; i++) {
        UntypedVecPush(&vec, a, sizeof(i32), alignof(i32), &nums[i]);
    }

    i64 index = 5;
    while (vec.length != 0) {
        const i32* top = UntypedVecPop(&vec, sizeof(i32));
        Assert(*top == nums[index]);
        index--;
    }

    UntypedVecFree(&vec, a, sizeof(i32), alignof(i32));
 
    puts("untyped vec tests passed\n");
}

void TestUntypedHashmap(void) {
    const Allocator a = StdAllocator();

    String ryan = "Ryan";
    String aidan = "Aidan";
    String codebase = "Codebase";
    String ringo = "ringo";

    UntypedHashmap map = UntypedHashmapNew(UntypedStringEqual, UntypedStringHash);

    Assert(UntypedHashmapGet(&map, sizeof(String), &ryan, sizeof(i32)) == NULL);
    Assert(UntypedHashmapGet(&map, sizeof(String), &aidan, sizeof(i32)) == NULL);

    UntypedHashmapSet(&map, a, sizeof(String), alignof(String), &ryan, sizeof(i32), alignof(i32), &(i32){20});
    i32* v1 = UntypedHashmapGet(&map, sizeof(String), &ryan, sizeof(i32));
    Assert(v1 != NULL);
    Assert(*v1 == 20);

    UntypedHashmapSet(&map, a, sizeof(String), alignof(String), &aidan, sizeof(i32), alignof(i32), &(i32){17});
    i32* v2 = UntypedHashmapGet(&map, sizeof(String), &aidan, sizeof(i32));
    Assert(v2 != NULL);
    Assert(*v2 == 17);

    i32* v3 = UntypedHashmapGet(&map, sizeof(String), &codebase, sizeof(i32));
    Assert(v3 == NULL);

    UntypedHashmapSet(&map, a, sizeof(String), alignof(String), &ryan, sizeof(i32), alignof(i32), &(i32){21});
    i32* v4 = UntypedHashmapGet(&map, sizeof(String), &ryan, sizeof(i32));
    Assert(v4 != NULL);
    Assert(*v4 == 21);

    Assert(UntypedHashmapGet(&map, sizeof(String), &codebase, sizeof(i32)) == NULL);
    Assert(UntypedHashmapGet(&map, sizeof(String), &ringo, sizeof(i32)) == NULL);

    Assert(!UntypedHashmapTryRemove(&map, sizeof(String), &codebase, sizeof(i32), NULL));
    Assert(!UntypedHashmapTryRemove(&map, sizeof(String), &ringo, sizeof(i32), NULL));

    void* out = NULL;
    Assert(UntypedHashmapTryRemove(&map, sizeof(String), &ryan, sizeof(i32), &out));
    Assert(*(i32*)out == 21);

    Assert(UntypedHashmapGet(&map, sizeof(String), &ryan, sizeof(i32)) == NULL);

    UntypedHashmapSet(&map, a, sizeof(String), alignof(String), &ryan, sizeof(i32), alignof(i32), &(i32){21});
    UntypedHashmapSet(&map, a, sizeof(String), alignof(String), &codebase, sizeof(i32), alignof(i32), &(i32){39});
    UntypedHashmapSet(&map, a, sizeof(String), alignof(String), &ringo, sizeof(i32), alignof(i32), &(i32){12});

    UntypedHashmapIterator it = UntypedHashmapIteratorNew(&map);

    Assert(UntypedHashmapIteratorNext(&it, sizeof(String), sizeof(i32)));
    Assert(UntypedHashmapIteratorNext(&it, sizeof(String), sizeof(i32)));
    Assert(UntypedHashmapIteratorNext(&it, sizeof(String), sizeof(i32)));
    Assert(UntypedHashmapIteratorNext(&it, sizeof(String), sizeof(i32)));
    Assert(!UntypedHashmapIteratorNext(&it, sizeof(String), sizeof(i32)));
    Assert(!UntypedHashmapIteratorNext(&it, sizeof(String), sizeof(i32)));

    UntypedHashmapFree(&map, a, sizeof(String), alignof(String), sizeof(i32), alignof(i32));

    puts("untyped hashmap tests passed\n");
}

void TestIntrusiveList(void) {
    typedef struct Data {
        i32 num;
        IntrusiveNode node;
    } Data;

    IntrusiveList list = {0};
    Data one = {1, {0}};
    Data two = {2, {0}};
    Data three = {3, {0}};
    Data four = {4, {0}};
    Data five = {5, {0}};
    IntrusiveListPushFront(&list, &two.node);
    IntrusiveNodeInsertAfter(&two.node, &five.node);
    IntrusiveListPushFront(&list, &one.node);
    IntrusiveNodeInsertAfter(&two.node, &three.node);
    IntrusiveNodeInsertAfter(&three.node, &four.node);

    {
        IntrusiveNode* curr = list.head;
        i64 count = 0;
        while (curr) {
            count++;
            curr = curr->next;
        }
        Assert(count == 5);
    }
    {
        IntrusiveNode* curr = list.head;
        i32 index = 1;
        while (curr) {
            const Data* d = ParentOf(Data, node, curr);
            Assert(d->num == index);
            index++;
            curr = curr->next;
        }
    }

    puts("intrusive list tests passed\n");
}

