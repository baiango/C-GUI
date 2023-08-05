#pragma once
#include <cstdint>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


struct Vec2i { int32_t x, y; };
struct Vec2u { uint32_t x, y; };
struct Vec2f32 { float_t x, y; };
struct Vec2f64 { double_t x, y; };

struct Vec3i { int32_t x, y, z; };
struct Vec3u { uint32_t x, y, z; };

struct col8 { uint8_t r, g, b; };
struct col8a { uint8_t r, g, b, a; };
struct col5 { uint8_t rgb; };

struct perspective {
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);
};
