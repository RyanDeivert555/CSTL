#pragma once
#include "common.h"

// TODO: should ptr be u8*?
typedef void (*random_bytes_func)(void* ctx, void* ptr, i64 size);

void set_global_random(void* ctx, random_bytes_func random_bytes);
void set_random_bytes(void* buffer, i64 size);
void std_random_bytes(void* ctx, void* buffer, i64 size);
