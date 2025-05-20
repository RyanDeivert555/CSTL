#pragma once
#include "allocator.h"
#include <stdint.h>

typedef struct fba {
    u8* buffer;
    i64 size;
    i64 capacity;
} fba;

fba fba_new(u8* buffer, i64 capacity);
allocator fba_as_allocator(fba* fba);

