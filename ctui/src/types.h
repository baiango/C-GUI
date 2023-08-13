#pragma once
#include <stdint.h>


struct Vec2i { int32_t x, y; };
struct Vec2u { uint32_t x, y; };
struct Vec2f { float x, y; };
struct Vec2f64 { double x, y; };

struct Vec3i { int32_t x, y, z; };
struct Vec3f { float x, y, z; };
struct Vec3f64 { double x, y, z; };
struct Vec3u { uint32_t x, y, z; };

struct Col8 { uint8_t r, g, b; };
struct Col8a { uint8_t r, g, b, a; };
struct Col5a { uint16_t rgba; };