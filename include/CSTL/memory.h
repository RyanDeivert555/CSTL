#pragma once
#include "CSTL/common.h"
#include <string.h> // IWYU pragma: keep

void cstl_raw_memcpy(void* restrict dest, const void* restrict src, i64 size, i64 count);
void cstl_raw_memmove(void* dest, const void* src, i64 size, i64 count);
bool cstl_raw_memcmp(const void* p0, const void* p1, i64 size, i64 count);

#define cstl_memcpy(T, dest, src, count) cstl_raw_memcpy(dest, src, sizeof(T), count)
#define cstl_memmove(T, dest, src, count) cstl_raw_memmove(dest, src, sizeof(T), count)
#define cstl_memcmp(T, p0, p1, count) cstl_raw_memcmp(p0, p1, sizeof(T), count)
#define cstl_bitcast(T, v) (*(T*)memcpy(&(T){0}, &(v), sizeof(T)))
#define cstl_array_bitcast(T, dest, src) (memcpy(dest, &(src), sizeof(T)))
