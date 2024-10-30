#include "../include/fba.h"
#include <stdint.h>

fba fba_new(unsigned char* buffer, size_t capacity) {
    fba result = {
        .buffer = buffer,
        .capacity = capacity,
        .size = 0,
    };

    return result;
}

static bool fba_is_last_alloc(fba* instance, unsigned char* buffer, size_t length) {
    return buffer + length == instance->buffer + instance->size;
}

static unsigned char* fba_alloc(void* ctx, size_t bytes, size_t align) {
    fba* instance = (fba*)ctx;

    unsigned char* current = instance->buffer + instance->size;
    size_t padding = -(uintptr_t)current & (align - 1);
    size_t offset = padding + bytes;
    
    if (instance->size + offset > instance->capacity) {
        return NULL;
    }

    unsigned char* data = current + padding;
    instance->size += offset;

    return data;
}

static bool fba_realloc(void* ctx, unsigned char** ptr, size_t current_size, size_t new_size, size_t align) {
    fba* instance = (fba*)ctx;

    if (!fba_is_last_alloc(instance, *ptr, current_size)) {
        if (new_size > current_size) {
            return false;
        } else {
            return true;
        }
    }
    
    if (new_size <= current_size) {
        size_t sub = current_size - new_size;
        instance->size -= sub;

        return true;
    }

    size_t add = new_size - current_size;

    if (add + instance->size > instance->capacity) {
        return false;
    }

    instance->size += add;

    return true;
}

static void fba_free(void* ctx, unsigned char* ptr, size_t size, size_t align) {
    fba* instance = (fba*)ctx;

    if (fba_is_last_alloc(instance, ptr, size)) {
        instance->size -= size;
    }
}

static allocator_vtable fba_vtable = {
    .alloc = fba_alloc,
    .realloc = fba_realloc,
    .free = fba_free,
};

allocator fba_as_allocator(fba* fba) {
    allocator result = {
        .ctx = fba,
        .vtable = &fba_vtable,
    };

    return result;
}

