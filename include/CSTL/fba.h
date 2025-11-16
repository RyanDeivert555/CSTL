#pragma once
#include "allocator.h"

typedef struct Fba {
    U8* buffer;
    I64 size;
    I64 capacity;
} Fba;

Fba FbaNew(U8* buffer, I64 capacity);
Allocator FbaAsAllocator(Fba* fba);
