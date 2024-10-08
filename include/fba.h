#pragma once
#include "allocator.h"

// TODO: fba not portable, requires -fno-strict-aliasing
typedef struct fba {
    unsigned char* buffer;
    size_t length;
    size_t end_index;
} fba;

fba fba_new(unsigned char* buffer, size_t length);
allocator fba_as_allocator(fba* fba);

