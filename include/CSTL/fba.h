#pragma once
#include "allocator.h"
#include "common.h"

typedef struct fba {
    cstl_byte* buffer;
    isize size;
    isize capacity;
} fba;

fba fba_new(cstl_byte* buffer, isize capacity);
allocator fba_as_allocator(fba* instance);
