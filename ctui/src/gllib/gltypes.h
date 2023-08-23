#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


// gltypes.c Global //
// gltypes.c Struct //
struct Mesh
{	uint16_t row;
	GLfloat* vertices;
	GLuint* indices;
	uint16_t sizeof_vertices;
	uint16_t sizeof_indices;
	GLuint vao, vbo, ebo, shader_program;
	uint16_t index_attribute;
	uint16_t prefix_sum_attribute; };

struct PointerPoolGL
{	GLuint* VAOs, * VBOs, * EBOs;
	GLuint* shader_programs; };

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

struct Mat4 { float data[16]; };
// gltypes.c Function //
void cgui_prt_gl_error();
void cgui_unbind_all();
char* cgui_read_file(char* path);
GLuint cgui_init_shaders(char* vertex_path, char* fragment_path);
void cgui_set_uniform1f(char* name, float param0, GLuint shader_program);
void cgui_set_uniform2f
(	char* name,
    float param0, float param1,
    GLuint shader_program);
void cgui_set_uniform3f
(	char* name,
    float param0, float param1, float param2,
    GLuint shader_program);
void cgui_cook_vertices(struct Mesh* mesh);
struct Mesh cgui_add_attribute(GLint size, struct Mesh* mesh);
void cgui_set_vec3f_from_floats(struct Vec3f* vec, float x, float y, float z);
void cgui_set_vec3f_from_vec3f(struct Vec3f* dest, const struct Vec3f* src);
void cgui_prt_vec3f(struct Vec3f* vec);
void cgui_set_mat4
(	struct Mat4* mat,
    float m00, float m01, float m02, float m03,
    float m10, float m11, float m12, float m13,
    float m20, float m21, float m22, float m23,
    float m30, float m31, float m32, float m33 );
void cgui_zero_mat4(struct Mat4* mat);
void cgui_set_diagonal_mat4(struct Mat4* mat, float flt);
void cgui_prt_mat4(struct Mat4* mat);
