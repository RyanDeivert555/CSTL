#include "CSTL/fba.h"
#include "CSTL/allocator.h"

fba fba_new(u8* buffer, i64 capacity) {
    const fba result = {
        .buffer = buffer,
        .capacity = capacity,
        .size = 0,
    };

    return result;
}

static u8* fba_alloc(void* ctx, i64 size, i64 count, i64 align) {
    fba* instance = ctx;

    u8* current = instance->buffer + instance->size;
    const i64 padding = -(usize)current & (align - 1);
    const i64 offset = padding + (size * count);

    if (instance->size + offset > instance->capacity) {
        return NULL;
    }

    u8* data = current + padding;
    instance->size += offset;

    return data;
}

static bool fba_is_last_alloc(fba* self, u8* buffer, i64 size) {
    return buffer + size == self->buffer + self->size;
}

static void fba_free(void* ctx, u8* ptr, i64 size, i64 count, i64 align) {
    (void)align;

    fba* instance = ctx;

    if (fba_is_last_alloc(instance, ptr, size * count)) {
        instance->size -= size * count;
    }
}

static const allocator_v_table fba_vtable = {
    .alloc = fba_alloc,
    .free = fba_free,
};

allocator fba_as_allocator(fba* fba) {
    const allocator result = {
        .ctx = fba,
        .vtable = &fba_vtable,
    };

    return result;
}
