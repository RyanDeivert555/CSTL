#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;
typedef int8_t I8;
typedef int16_t I16;
typedef int32_t I32;
typedef int64_t I64;
typedef ptrdiff_t Isize;
typedef size_t Usize;
typedef float F32;
typedef double F64;

#define Assert(c) while (!(c)) __builtin_unreachable()

