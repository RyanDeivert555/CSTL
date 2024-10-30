#pragma once
#include "allocator.h"

typedef struct fba {
    unsigned char* buffer;
    size_t size;
    size_t capacity;
} fba;

fba fba_new(unsigned char* buffer, size_t capacity);
allocator fba_as_allocator(fba* fba);

