#include "allocator.h"
#include "common.h"
#include "fba.h"
#include "hashmap.h"
#include "intrusive_list.h"
#include "untyped_hashmap.h"
#include "untyped_vec.h"
#include "vec.h"
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
I64 StringHash(String key) {
    I64 result = 5381;

    while (*key) {
        result = ((result << 5) + result) + *key;

        key++;
    }

    return result;
}

bool StringEqual(String a, String b) {
    return strcmp(a, b) == 0;
}

I64 UntypedStringHash(const void* key) {
    return StringHash(*(String*)key);
}

bool UntypedStringEqual(const void* a, const void* b) {
    return StringEqual(*(String*)a, *(String*)b);
}

VEC_DEFINE(U8);
VEC_IMPL(U8);
HASHMAP_DEFINE(String, I32);
HASHMAP_IMPL(String, I32, StringEqual, StringHash);

void TestVec(void) {
    const Allocator a = StdAllocator();

    VecU8 vec = {0};
    const char* hello = "gdkkn";
    for (I64 i = 0; i < 5; i++) {
        VecU8Push(&vec, a, hello[i]);
    }

    for (I64 i = 0; i < vec.length; i++) {
        vec.buffer[i] += 1;
    }
    VecU8Push(&vec, a, '\n');

    const char* expected = "hello\n";
    for (I64 i = 0; i < vec.length; i++) {
        Assert(expected[i] == vec.buffer[i]);
    }

    VecU8Free(&vec, a);

    puts("vec tests passed\n");
}

void TestHashmap(void) {
    const Allocator a = StdAllocator();

    HashmapStringI32 map = {0};

    Assert(HashmapStringI32Get(&map, "Ryan") == NULL);
    Assert(HashmapStringI32Get(&map, "Aidan") == NULL);

    HashmapStringI32Set(&map, a, "Ryan", 19);
    HashmapStringI32Set(&map, a, "Aidan", 16);

    I32* v1 = HashmapStringI32Get(&map, "Ryan");
    Assert(v1 != NULL);
    Assert(*v1 == 19);

    I32* v2 = HashmapStringI32Get(&map, "Aidan");
    Assert(v2 != NULL);
    Assert(*v2 == 16);

    I32* v3 = HashmapStringI32Get(&map, "Bob");
    Assert(v3 == NULL);

    HashmapStringI32Set(&map, a, "Ryan", 21);
    I32* v4 = HashmapStringI32Get(&map, "Ryan");
    Assert(v4 != NULL);
    Assert(*v4 == 21);

    Assert(HashmapStringI32Get(&map, "Bobo") == NULL);
    Assert(HashmapStringI32Get(&map, "Momo") == NULL);

    Assert(!HashmapStringI32TryRemove(&map, "Bobo", NULL));
    Assert(!HashmapStringI32TryRemove(&map, "Momo", NULL));

    I32 out;
    Assert(HashmapStringI32TryRemove(&map, "Ryan", &out));
    Assert(out == 21);
    Assert(map.count == 1);

    Assert(HashmapStringI32Get(&map, "Ryan") == NULL);

    HashmapStringI32Set(&map, a, "Ryan", 21);
    HashmapStringI32Set(&map, a, "Momo", 39);
    HashmapStringI32Set(&map, a, "Bobo", 12);
    Assert(map.count == 4);

    HashmapStringI32Iterator it = HashmapStringI32IteratorNew(&map);

    Assert(HashmapStringI32IteratorNext(&it));
    Assert(HashmapStringI32IteratorNext(&it));
    Assert(HashmapStringI32IteratorNext(&it));
    Assert(HashmapStringI32IteratorNext(&it));
    Assert(!HashmapStringI32IteratorNext(&it));
    Assert(!HashmapStringI32IteratorNext(&it));

    Assert(HashmapStringI32TryRemove(&map, "Ryan", &out));
    Assert(out == 21);
    Assert(HashmapStringI32TryRemove(&map, "Aidan", &out));
    Assert(out == 16);
    Assert(HashmapStringI32TryRemove(&map, "Momo", &out));
    Assert(out == 39);
    Assert(HashmapStringI32TryRemove(&map, "Bobo", &out));
    Assert(out == 12);
    Assert(map.count == 0);
    Assert(!HashmapStringI32TryRemove(&map, "Bobo", NULL));
    Assert(map.count == 0);

    HashmapStringI32Free(&map, a);

    puts("hashmap tests passed\n");
}

void TestAllocator(void) {
    const Allocator a = StdAllocator();

    {
        I32* num = (I32*)AllocatorRawAlloc(a, sizeof(I32), 1, alignof(I32));
        Assert(num != NULL);
        Usize address = (Usize)num;
        Assert(address % alignof(I32) == 0);

        *num = 0;

        Assert(*num == 0);

        Assert(num != NULL);
        address = (Usize)num;
        Assert(address % alignof(I32) == 0);

        num[0] = 1;
        Assert(num[0] == 1);

        AllocatorRawFree(a, (U8*)num, sizeof(I32), 1, alignof(I32));
    }
    {
        const Usize len = 10;
        U64* nums = AllocatorAlloc(U64, a, len);
        const Usize address = (Usize)nums;
        Assert(address % alignof(U64) == 0);

        for (U64 i = 0; i < len; i++) {
            nums[i] = i;
        }
        for (U64 i = 0; i < len; i++) {
            Assert(nums[i] == i);
        }

        AllocatorFree(U64, a, nums, len);
    }
    {
        U64* ptr = AllocatorAlloc(U64, a, 0);
        Assert(ptr != NULL);
        AllocatorFree(U64, a, ptr, 0);
    }

    puts("allocator test passed\n");
}

void TestFba(void) {
    U8 buffer[1 << 16];
    Fba fba = FbaNew(buffer, sizeof(buffer));
    const Allocator a = FbaAsAllocator(&fba);

    {
        I32* memory = AllocatorAlloc(I32, a, 1);
        const Usize address = (Usize)memory;
        Assert(address % alignof(I32) == 0);

        Assert(memory != NULL);
        *memory = 10;

        AllocatorFree(I32, a, memory, 1);
        const Fba* instance = a.ctx;
        // Free last allocation
        Assert(instance->size == 0);
    }
    {
        const Usize length = 10;
        I32* memory = AllocatorAlloc(I32, a, length);

        Assert(memory != NULL);
        AllocatorFree(I32, a, memory, length);
        const Fba* instance = a.ctx;
        Assert(instance->size == 0);
    }
    {
        const I32* first = AllocatorAlloc(I32, a, 10);
        const I32* second = AllocatorAlloc(I32, a, 10);
        AllocatorFree(I32, a, first, 10);

        const Fba* instance = a.ctx;
        // not last allocation, no freeing
        Assert(instance->size == 20 * sizeof(I32));
        AllocatorFree(I32, a, second, 10);
        Assert(instance->size == 10 * sizeof(I32));
        AllocatorFree(I32, a, first, 10);
        Assert(instance->size == 0);
    }
    {
        U64* ptr = AllocatorAlloc(U64, a, 0);
        Assert(ptr != NULL);
        AllocatorFree(U64, a, ptr, 0);
    }

    puts("fba test passed\n");
}

void TestUntypedVec(void) {
    const Allocator a = StdAllocator();

    UntypedVec vec = {0};
    const char* hello = "gdkkn";
    for (I64 i = 0; i < 5; i++) {
        UntypedVecPush(&vec, a, sizeof(U8), alignof(U8), &hello[i]);
    }

    for (I64 i = 0; i < vec.length; i++) {
        U8* item = (U8*)UntypedVecGet(&vec, i, sizeof(U8));
        *item += 1;
    }
    UntypedVecPush(&vec, a, sizeof(U8), alignof(U8), &(U8){'\n'});

    const U8* expected = (const U8*)"hello\n";
    for (I64 i = 0; i < vec.length; i++) {
        const U8 c = *(U8*)UntypedVecGet(&vec, i, sizeof(U8));
        Assert(expected[i] == c);
    }

    UntypedVecRemove(&vec, vec.length - 1, sizeof(U8));

    const U8* last = UntypedVecPop(&vec, sizeof(U8));
    Assert(*last == 'o');

    const U8* first = UntypedVecGet(&vec, 0, sizeof(U8));
    Assert(*first == 'h');

    UntypedVecFree(&vec, a, sizeof(U8), alignof(U8));

    // test with integers now
    memset(&vec, 0, sizeof(UntypedVec));
    Assert(!vec.buffer && !vec.length && !vec.capacity);

    const I32 nums[] = {0, 1, 2, 4, 8, 16, -1};
    for (I64 i = 0; nums[i] != -1; i++) {
        UntypedVecPush(&vec, a, sizeof(I32), alignof(I32), &nums[i]);
    }

    I64 index = 5;
    while (vec.length != 0) {
        const I32* top = UntypedVecPop(&vec, sizeof(I32));
        Assert(*top == nums[index]);
        index--;
    }

    UntypedVecFree(&vec, a, sizeof(I32), alignof(I32));

    puts("untyped vec tests passed\n");
}

void TestUntypedHashmap(void) {
    const Allocator a = StdAllocator();

    String ryan = "Ryan";
    String aidan = "Aidan";
    String codebase = "Codebase";
    String ringo = "ringo";

    UntypedHashmap map = UntypedHashmapNew(UntypedStringEqual, UntypedStringHash);

    Assert(UntypedHashmapGet(&map, sizeof(String), &ryan, sizeof(I32)) == NULL);
    Assert(UntypedHashmapGet(&map, sizeof(String), &aidan, sizeof(I32)) == NULL);

    UntypedHashmapSet(&map, a, sizeof(String), alignof(String), &ryan, sizeof(I32), alignof(I32), &(I32){20});
    I32* v1 = UntypedHashmapGet(&map, sizeof(String), &ryan, sizeof(I32));
    Assert(v1 != NULL);
    Assert(*v1 == 20);

    UntypedHashmapSet(&map, a, sizeof(String), alignof(String), &aidan, sizeof(I32), alignof(I32), &(I32){17});
    I32* v2 = UntypedHashmapGet(&map, sizeof(String), &aidan, sizeof(I32));
    Assert(v2 != NULL);
    Assert(*v2 == 17);

    I32* v3 = UntypedHashmapGet(&map, sizeof(String), &codebase, sizeof(I32));
    Assert(v3 == NULL);

    UntypedHashmapSet(&map, a, sizeof(String), alignof(String), &ryan, sizeof(I32), alignof(I32), &(I32){21});
    I32* v4 = UntypedHashmapGet(&map, sizeof(String), &ryan, sizeof(I32));
    Assert(v4 != NULL);
    Assert(*v4 == 21);

    Assert(UntypedHashmapGet(&map, sizeof(String), &codebase, sizeof(I32)) == NULL);
    Assert(UntypedHashmapGet(&map, sizeof(String), &ringo, sizeof(I32)) == NULL);

    Assert(!UntypedHashmapTryRemove(&map, sizeof(String), &codebase, sizeof(I32), NULL));
    Assert(!UntypedHashmapTryRemove(&map, sizeof(String), &ringo, sizeof(I32), NULL));

    void* out = NULL;
    Assert(UntypedHashmapTryRemove(&map, sizeof(String), &ryan, sizeof(I32), &out));
    Assert(*(I32*)out == 21);

    Assert(UntypedHashmapGet(&map, sizeof(String), &ryan, sizeof(I32)) == NULL);

    UntypedHashmapSet(&map, a, sizeof(String), alignof(String), &ryan, sizeof(I32), alignof(I32), &(I32){21});
    UntypedHashmapSet(&map, a, sizeof(String), alignof(String), &codebase, sizeof(I32), alignof(I32), &(I32){39});
    UntypedHashmapSet(&map, a, sizeof(String), alignof(String), &ringo, sizeof(I32), alignof(I32), &(I32){12});

    UntypedHashmapIterator it = UntypedHashmapIteratorNew(&map);

    Assert(UntypedHashmapIteratorNext(&it, sizeof(String), sizeof(I32)));
    Assert(UntypedHashmapIteratorNext(&it, sizeof(String), sizeof(I32)));
    Assert(UntypedHashmapIteratorNext(&it, sizeof(String), sizeof(I32)));
    Assert(UntypedHashmapIteratorNext(&it, sizeof(String), sizeof(I32)));
    Assert(!UntypedHashmapIteratorNext(&it, sizeof(String), sizeof(I32)));
    Assert(!UntypedHashmapIteratorNext(&it, sizeof(String), sizeof(I32)));

    UntypedHashmapFree(&map, a, sizeof(String), alignof(String), sizeof(I32), alignof(I32));

    puts("untyped hashmap tests passed\n");
}

void TestIntrusiveList(void) {
    typedef struct Data {
        I32 num;
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
        I64 count = 0;
        while (curr) {
            count++;
            curr = curr->next;
        }
        Assert(count == 5);
    }
    {
        IntrusiveNode* curr = list.head;
        I32 index = 1;
        while (curr) {
            const Data* d = ParentOf(Data, node, curr);
            Assert(d->num == index);
            index++;
            curr = curr->next;
        }
    }

    puts("intrusive list tests passed\n");
}
