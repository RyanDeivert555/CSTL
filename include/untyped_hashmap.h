#pragma once
#include "common.h"
#include "allocator.h"

typedef bool (*CompareFunc)(const void* lhs, const void* rhs);
typedef i64 (*HashFunc)(const void* key);

typedef enum UntypedHashmapState {
    STATE_EMPTY,
    STATE_OCCUPIED,
} UntypedHashmapState;

typedef struct UntypedHashmap {
    CompareFunc compare;
    HashFunc hash;
    void* keys;
    void* values;
    UntypedHashmapState* states;
    i64 capacity;
    i64 count;
} UntypedHashmap;

UntypedHashmap UntypedHashmapNew(CompareFunc compare, HashFunc hash);
void UntypedHashmapFree(UntypedHashmap* map, Allocator allocator, i64 key_size, i64 key_align, i64 value_size, i64 value_align);
void UntypedHashmapRealloc(UntypedHashmap* map, Allocator allocator, i64 key_size, i64 key_align, i64 value_size, i64 value_align, i64 new_size);
void UntypedHashmapSet(UntypedHashmap* map, Allocator allocator, i64 key_size, i64 key_align, const void* const key, i64 value_size, i64 value_align, const void* const value);
void* UntypedHashmapGet(UntypedHashmap* map, i64 key_size, const void* const key, i64 value_size);

