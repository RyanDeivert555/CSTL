#include "untyped_hashmap.h"
#include "allocator.h"
#include "common.h"

UntypedHashmap UntypedHashmapNew(CompareFunc compare, HashFunc hash) {
    UntypedHashmap result = {0};
    result.compare = compare;
    result.hash = hash;

    return result;
}

void UntypedHashmapFree(UntypedHashmap* map, Allocator allocator, i64 key_size, i64 key_align, i64 value_size, i64 value_align) {
    AllocatorRawFree(allocator, (u8*)map->keys, key_size, map->capacity, key_align);
    AllocatorRawFree(allocator, (u8*)map->values, value_size, map->capacity, value_align);
    AllocatorFree(UntypedHashmapState, allocator, map->states, map->capacity);
}

void UntypedHashmapSet(UntypedHashmap* map, Allocator allocator, i64 key_suze, i64 key_align, const void* const key, i64 value_size, i64 value_align, const void* const value) {

}
void* UntypedHashmapGet(UntypedHashmap* map, const void* const key, i64 value_size) {
    if (map->capacity == 0) {
        return NULL;
    }

    const i64 index = map->hash(key) % map->capacity;
    if (index < 0 || index >= map->capacity) {
        return NULL;
    }

    return (u8*)map->values + index * value_size;
}

