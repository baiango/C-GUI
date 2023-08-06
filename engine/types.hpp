#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


struct vec2i { int32_t x, y; };
struct vec2u { uint32_t x, y; };
struct vec2f32 { float_t x, y; };
struct vec2f64 { double_t x, y; };

struct vec3i { int32_t x, y, z; };
struct vec3u { uint32_t x, y, z; };

struct col8 { uint8_t r, g, b; };
struct col8a { uint8_t r, g, b, a; };
struct col5 { uint8_t rgb; };

struct perspective {
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);
};
