#include "../include/allocator.h"
#include <stdlib.h>
#include <assert.h>

char* allocator_raw_alloc(allocator allocator, size_t bytes) {
    return allocator.vtable->alloc(allocator.ctx, bytes);
}

bool allocator_raw_realloc(allocator allocator, char** ptr, size_t current_size, size_t new_size) {
    return allocator.vtable->realloc(allocator.ctx, ptr, current_size, new_size);
}

void allocator_raw_free(allocator allocator, char* ptr, size_t size) {
    allocator.vtable->free(allocator.ctx, ptr, size);
}

static char* c_alloc(void* ctx, size_t bytes) {
    (void)ctx;
    
    return malloc(bytes);
}

static bool c_realloc(void* ctx, char** ptr, size_t current_size, size_t new_size) {
    (void)ctx;
    (void)current_size;

    char* result = realloc(*ptr, new_size);

    if (result != NULL) {
        *ptr = result; 

        return true;
    } else {
        *ptr = NULL;

        return false;
    }
}

static void c_free(void* ctx, char* ptr, size_t size) {
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
