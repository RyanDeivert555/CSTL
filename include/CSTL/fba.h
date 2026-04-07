#pragma once
#include "common.h"

typedef struct fba {
    u8* buffer;
    i64 size;
    i64 capacity;
} fba;

fba fba_new(u8* buffer, i64 capacity);
u8* fba_alloc(void* ctx, i64 size, i64 count, i64 align);
void fba_free(void* ctx, u8* ptr, i64 size, i64 count, i64 align);
