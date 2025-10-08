#pragma once
#include "common.h"
#include "allocator.h"

typedef bool (*CompareFunc)(const void* lhs, const void* rhs);
typedef I64 (*HashFunc)(const void* key);

typedef enum UntypedHashmapState {
    UNTYPED_HASHMAP_STATE_EMPTY,
    UNTYPED_HASHMAP_STATE_OCCUPIED,
    UNTYPED_HASHMAP_STATE_TOMBSTONE,
} UntypedHashmapState;

typedef struct UntypedHashmap {
    CompareFunc compare;
    HashFunc hash;
    void* keys;
    void* values;
    UntypedHashmapState* states;
    I64 capacity;
    I64 count;
} UntypedHashmap;

UntypedHashmap UntypedHashmapNew(CompareFunc compare, HashFunc hash);
void UntypedHashmapFree(UntypedHashmap* map, Allocator allocator, I64 key_size, I64 key_align, I64 value_size, I64 value_align);
void UntypedHashmapRealloc(UntypedHashmap* map, Allocator allocator, I64 key_size, I64 key_align, I64 value_size, I64 value_align, I64 new_size);
void UntypedHashmapSet(UntypedHashmap* map, Allocator allocator, I64 key_size, I64 key_align, const void* const key, I64 value_size, I64 value_align, const void* const value);
void* UntypedHashmapGet(UntypedHashmap* map, I64 key_size, const void* const key, I64 value_size);
bool UntypedHashmapTryRemove(UntypedHashmap* map, I64 key_size, const void* const key, I64 value_size, void** out_value);

typedef struct UntypedHashmapIterator {
    const UntypedHashmap* inner;
    void* key;
    void* value;
    I64 index;
} UntypedHashmapIterator;

UntypedHashmapIterator UntypedHashmapIteratorNew(const UntypedHashmap* map);
bool UntypedHashmapIteratorNext(UntypedHashmapIterator* it, I64 key_size, I64 value_size);

