#pragma once
#include <glad/glad.h>
#include <string>


std::string cgui_get_file_content(std::string file_name);
GLuint cgui_init_shaders(std::string vertex_file, std::string fragment_file);
void cgui_prtGLError();
void cgui_unbindAll();


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
struct Perspective {
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);
};

void cgui_shader_update_view(GLuint shader_program, Perspective *camera);

class Mesh {
public:
	GLuint vao{}, vbo{}, ebo{}, shader_program{};
	GLfloat *vertices = nullptr;
	GLuint *indices = nullptr;
	uint32_t vertices_sizeof = 0, indices_sizeof = 0;
	uint32_t row_vertices = 0;
	uint32_t index_vertex_attribute = 0;
	uint32_t prefix_sum_vertex_attribute = 0;

	Mesh &set_vertices(uint32_t row, GLfloat *vertices, uint32_t vertices_sizeof);
	Mesh &set_indices(GLuint *indices, uint32_t indices_sizeof);
	Mesh &add_attribute(GLint size);
	Mesh &cook_vertices();
};

class CguiRender {
public:
	CguiRender &bind_texture(GLuint texture);

	CguiRender &render(
		GLuint vao, GLuint shader_program,
		GLsizei indices_size, Perspective *camera
	);
};
