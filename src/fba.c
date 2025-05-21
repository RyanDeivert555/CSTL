#include "fba.h"
#include "allocator.h"

Fba FbaNew(u8* buffer, i64 capacity) {
    const Fba result = {
        .buffer = buffer,
        .capacity = capacity,
        .size = 0,
    };

    return result;
}

static u8* FbaAlloc(void* ctx, i64 size, i64 count, i64 align) {
    Fba* instance = (Fba*)ctx;

    u8* current = instance->buffer + instance->size;
    const i64 padding = -(u64)current & (align - 1);
    const i64 offset = padding + (size * count);
    
    if (instance->size + offset > instance->capacity) {
        return NULL;
    }

    u8* data = current + padding;
    instance->size += offset;

    return data;
}

static void FbaFree(void* ctx, u8* ptr, i64 size, i64 count, i64 align) {
    (void)ctx;
    (void)ptr;
    (void)size;
    (void)count;
    (void)align;
}

static const AllocatorVTable fba_vtable = {
    .alloc = FbaAlloc,
    .free = FbaFree,
};

Allocator FbaAsAllocator(Fba* Fba) {
    const Allocator result = {
        .ctx = Fba,
        .vtable = &fba_vtable,
    };

    return result;
}

