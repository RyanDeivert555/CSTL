#include "../include/allocator.h"
#include <stdlib.h>
#include <assert.h>

unsigned char* allocator_raw_alloc(allocator allocator, size_t bytes) {
    return allocator.vtable->alloc(allocator.ctx, bytes);
}

bool allocator_raw_realloc(allocator allocator, unsigned char** ptr, size_t current_size, size_t new_size) {
    return allocator.vtable->realloc(allocator.ctx, ptr, current_size, new_size);
}

void allocator_raw_free(allocator allocator, unsigned char* ptr, size_t size) {
    allocator.vtable->free(allocator.ctx, ptr, size);
}

static unsigned char* c_alloc(void* ctx, size_t bytes) {
    (void)ctx;
    
    return malloc(bytes);
}

static bool c_realloc(void* ctx, unsigned char** ptr, size_t current_size, size_t new_size) {
    (void)ctx;
    (void)current_size;

    unsigned char* result = realloc(*ptr, new_size);

    if (result != NULL) {
        *ptr = result; 

        return true;
    } else {
        *ptr = NULL;

        return false;
    }
}

static void c_free(void* ctx, unsigned char* ptr, size_t size) {
    (void)ctx;
    (void)size;
    free(ptr);
}

static allocator_vtable c_allocator_vtable = {
    .alloc = c_alloc,
    .realloc = c_realloc,
    .free = c_free,
};

allocator c_allocator(void) {
    allocator result = {
        .ctx = NULL,
        .vtable = &c_allocator_vtable,
    };

    return result;
}

fba fba_new(unsigned char* buffer, size_t length) {
    fba result = {
        .buffer = buffer,
        .length = length,
        .end_index = 0,
    };

    return result;
}

static bool fba_is_last_alloc(fba* instance, unsigned char* buffer, size_t length) {
    return buffer + length == instance->buffer + instance->end_index;
}

static unsigned char* fba_alloc(void* ctx, size_t bytes) {
    fba* instance = (fba*)ctx;

    size_t offset = instance->end_index;
    
    if (offset + bytes >= instance->length) {
        return NULL;
    } else {
        instance->end_index += bytes;

        return instance->buffer + offset;
    }
}

static bool fba_realloc(void* ctx, unsigned char** ptr, size_t current_size, size_t new_size) {
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
        instance->end_index -= sub;

        return true;
    }

    size_t add = new_size - current_size;

    if (add + instance->end_index > instance->length) {
        return false;
    }

    instance->end_index += add;

    return true;
}

static void fba_free(void* ctx, unsigned char* ptr, size_t size) {
    fba* instance = (fba*)ctx;

    if (fba_is_last_alloc(instance, ptr, size)) {
        instance->end_index -= size;
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
