#pragma once
#include "allocator.h"
#include <stdint.h>

typedef struct fba {
    uint8_t* buffer;
    int64_t size;
    int64_t capacity;
} fba;

fba fba_new(unsigned char* buffer, int64_t capacity);
allocator fba_as_allocator(fba* fba);

