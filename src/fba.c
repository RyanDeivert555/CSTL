#include "CSTL/fba.h"
#include "CSTL/allocator.h"
#include <stdbool.h>

Fba FbaNew(U8* buffer, I64 capacity) {
    const Fba result = {
        .buffer = buffer,
        .capacity = capacity,
        .size = 0,
    };

    return result;
}

static U8* FbaAlloc(void* ctx, I64 size, I64 count, I64 align) {
    Fba* instance = ctx;

    U8* current = instance->buffer + instance->size;
    const I64 padding = -(Usize)current & (align - 1);
    const I64 offset = padding + (size * count);

    if (instance->size + offset > instance->capacity) {
        return NULL;
    }

    U8* data = current + padding;
    instance->size += offset;

    return data;
}

static bool FbaIsLastAlloc(Fba* self, U8* buffer, I64 size) {
    return buffer + size == self->buffer + self->size;
}

static void FbaFree(void* ctx, U8* ptr, I64 size, I64 count, I64 align) {
    (void)align;

    Fba* instance = ctx;

    if (FbaIsLastAlloc(instance, ptr, size * count)) {
        instance->size -= size * count;
    }
}

static const AllocatorVTable fba_vtable = {
    .alloc = FbaAlloc,
    .free = FbaFree,
};

Allocator FbaAsAllocator(Fba* fba) {
    const Allocator result = {
        .ctx = fba,
        .vtable = &fba_vtable,
    };

    return result;
}
