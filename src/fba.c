#include "../include/fba.h"
#include <stdint.h>

fba fba_new(uint8_t* buffer, int64_t capacity) {
    const fba result = {
        .buffer = buffer,
        .capacity = capacity,
        .size = 0,
    };

    return result;
}

static uint8_t* fba_alloc(void* ctx, int64_t size, int64_t count, int64_t align) {
    fba* instance = (fba*)ctx;

    uint8_t* current = instance->buffer + instance->size;
    const int64_t padding = -(uintptr_t)current & (align - 1);
    const int64_t offset = padding + (size * count);
    
    if (instance->size + offset > instance->capacity) {
        return NULL;
    }

    uint8_t* data = current + padding;
    instance->size += offset;

    return data;
}

static void fba_free(void* ctx, uint8_t* ptr, int64_t size, int64_t count, int64_t align) {
    (void)ctx;
    (void)ptr;
    (void)size;
    (void)count;
    (void)align;
}

static allocator_vtable fba_vtable = {
    .alloc = fba_alloc,
    .free = fba_free,
};

allocator fba_as_allocator(fba* fba) {
    allocator result = {
        .ctx = fba,
        .vtable = &fba_vtable,
    };

    return result;
}

