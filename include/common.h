#pragma once
#include <stdint.h>
#include <stddef.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef size_t usize;
typedef float f32;
typedef double f64;

#define Assert(c) while (!(c)) __builtin_unreachable()

typedef struct TypeInfo {
    i64 size;
    i64 align;
} TypeInfo;

#define GetTypeInfo(T) (TypeInfo) { sizeof(T), _Alignof(T), }

