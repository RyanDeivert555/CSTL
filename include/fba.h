#pragma once
#include "allocator.h"

typedef struct Fba {
    u8* buffer;
    i64 size;
    i64 capacity;
} Fba;

Fba FbaNew(u8* buffer, i64 capacity);
Allocator FbaAsAllocator(Fba* fba);

