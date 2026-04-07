#include "CSTL/fba.h"

fba fba_new(u8* buffer, i64 capacity) {
    const fba result = {
        .buffer = buffer,
        .capacity = capacity,
        .size = 0,
    };

    return result;
}

u8* fba_alloc(void* ctx, i64 size, i64 count, i64 align) {
    fba* const instance = ctx;

    u8* const current = instance->buffer + instance->size;
    const i64 padding = -(usize)current & (align - 1);
    const i64 offset = padding + (size * count);

    if (instance->size + offset > instance->capacity) {
        return NULL;
    }

    u8* const data = current + padding;
    instance->size += offset;

    return data;
}

static bool fba_is_last_alloc(fba* self, u8* buffer, i64 size) {
    return buffer + size == self->buffer + self->size;
}

void fba_free(void* ctx, u8* ptr, i64 size, i64 count, i64 align) {
    (void)align;

    fba* const instance = ctx;

    if (fba_is_last_alloc(instance, ptr, size * count)) {
        instance->size -= size * count;
    }
}
