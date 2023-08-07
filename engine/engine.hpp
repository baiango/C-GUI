#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "types.hpp"

#include <string>
#include <vector>


struct Cam2D {
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);

	struct Vec2f scale = {2.0f, 2.0f}; // In range of -0.5f..0.5f
	float_t one_meter_size = -57.25f;
	float_t aspect_ratio{};

	Cam2D &process();
};

struct Cam3D {
	glm::vec3 position{};
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	int32_t width{}, height{};
	float_t aspect_ratio{};

	float_t speed = 0.05f;
	float_t sensitivity = 100.0f;

	Cam3D &process(
		int32_t cursor_state, Vec3i movement_input, Vec2f64 mouse_pos,
		Vec2u window_size, GLFWwindow *window
	);

};

class Mesh {
public:
	GLuint vao{}, vbo{}, ebo{}, shader_program{};
	GLfloat *vertices{};
	GLuint *indices{};
	uint32_t sizeof_vertices{}, sizeof_indices{};
	uint32_t row_vertices{};
	uint32_t index_vertex_attribute{};
	uint32_t prefix_sum_vertex_attribute{};

	Mesh &set_vertices(uint32_t row, GLfloat *vertices, uint32_t sizeof_vertices);
	Mesh &set_indices(GLuint *indices, uint32_t sizeof_indices);
	Mesh &add_attribute(GLint size);
	Mesh &cook_vertices();
};

class CguiRender {
public:
	CguiRender &bind_texture(GLuint texture);

	CguiRender &render(
		GLuint vao, GLuint shader_program,
		GLsizei indices_size, Cam2D *camera
	);

	CguiRender &render3D(
		GLuint vao, GLuint shader_program,
		GLsizei indices_size, Cam3D *camera
	);
};


std::string cgui_get_file_content(std::string file_name);
GLuint cgui_init_shaders(std::string vertex_file, std::string fragment_file);
void cgui_prtGLError();
void cgui_unbindAll();
void cgui_shader_update_view(GLuint shader_program, Cam2D *camera);
void cgui_shader_update_view3D(GLuint shader_program, Cam3D *camera);
void cgui_lsd_radix_sort(std::vector<int32_t> *start, std::vector<void *>comparator);
