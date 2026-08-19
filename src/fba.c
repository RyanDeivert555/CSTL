#include "CSTL/fba.h"
#include "CSTL/allocator.h"

fba fba_new(cstl_byte* buffer, isize capacity) {
    const fba result = {
        .buffer = buffer,
        .capacity = capacity,
        .size = 0,
    };

    return result;
}

static cstl_byte* fba_alloc(void* ctx, isize size, isize count, isize align) {
    fba* const instance = ctx;

    cstl_byte* const current = instance->buffer + instance->size;
    const isize padding = -(usize)current & (align - 1);
    const isize offset = padding + (size * count);

    if (instance->size + offset > instance->capacity) {
        return NULL;
    }

    cstl_byte* const data = current + padding;
    instance->size += offset;

    return data;
}

static bool fba_is_last_alloc(fba* self, cstl_byte* buffer, isize size) {
    return buffer + size == self->buffer + self->size;
}

static void fba_free(void* ctx, cstl_byte* ptr, isize size, isize count, isize align) {
    cstl_unused(align);

    fba* const instance = ctx;

    if (fba_is_last_alloc(instance, ptr, size * count)) {
        instance->size = ptr - instance->buffer;
    }
}

static const allocator_vtable vtable = {
    .alloc = fba_alloc,
    .free = fba_free,
};

allocator fba_as_allocator(fba* instance) {
    return (allocator){
        .ctx = instance,
        .vtable = &vtable,
    };
}
