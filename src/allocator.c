#include "../include/allocator.h"
#include <stdlib.h>

unsigned char* allocator_raw_alloc(allocator allocator, size_t bytes, size_t align) {
    return allocator.vtable->alloc(allocator.ctx, bytes, align);
}

bool allocator_raw_realloc(allocator allocator, unsigned char** ptr, size_t current_size, size_t new_size, size_t align) {
    return allocator.vtable->realloc(allocator.ctx, ptr, current_size, new_size, align);
}

void allocator_raw_free(allocator allocator, unsigned char* ptr, size_t size, size_t align) {
    allocator.vtable->free(allocator.ctx, ptr, size, align);
}

static unsigned char* c_alloc(void* ctx, size_t bytes, size_t align) {
    (void)ctx;
    (void)align;
    
    return malloc(bytes);
}

static bool c_realloc(void* ctx, unsigned char** ptr, size_t current_size, size_t new_size, size_t align) {
    (void)ctx;
    (void)current_size;
    (void)align;

    unsigned char* result = realloc(*ptr, new_size);

    if (result != NULL) {
        *ptr = result; 

        return true;
    } else {
        *ptr = NULL;

        return false;
    }
}

static void c_free(void* ctx, unsigned char* ptr, size_t size, size_t align) {
    (void)ctx;
    (void)size;
    (void)align;
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

