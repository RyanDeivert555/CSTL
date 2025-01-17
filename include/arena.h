#pragma once
#include "allocator.h"

typedef struct arena {
    allocator child_allocator;
} arena;

arena arena_new(allocator child_allocator);
allocator area_as_allocator(arena* arena);
void arena_free(arena* arena);

