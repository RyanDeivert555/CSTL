#include "CSTL/allocator.h"
#include <stdlib.h>

u8* cstl_raw_alloc(allocator a, i64 size, i64 count, i64 align) {
    return a.vtable->alloc(a.ctx, size, count, align);
}

void cstl_raw_free(allocator a, u8* ptr, i64 size, i64 count, i64 align) {
    a.vtable->free(a.ctx, ptr, size, count, align);
}

static u8* std_alloc(void* ctx, i64 size, i64 count, i64 align) {
    (void)ctx;
    (void)align;

    if (count <= 0 || size <= 0) {
        return NULL;
    }
    if (count > I64_MAX / size) {
        return NULL;
    }

    return (u8*)malloc(count * size);
}

static void std_free(void* ctx, u8* ptr, i64 size, i64 count, i64 align) {
    (void)ctx;
    (void)size;
    (void)count;
    (void)align;

    free(ptr);
}

static const allocator_vtable vtable = {
    .alloc = std_alloc,
    .free = std_free,
};

allocator std_allocator(void) {
    return (allocator){
        .ctx = NULL,
        .vtable = &vtable,
    };
}
