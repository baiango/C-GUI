#include "engine.hpp"
#include <iostream>
using std::cout;
using std::string;
using std::vector;
using std::array;
#include <fstream>


CguiRender &CguiRender::bind_texture(GLuint texture) {
	glBindTexture(GL_TEXTURE_2D, texture);
	return *this;
}

CguiRender &CguiRender::render(
	GLuint vao, GLuint shader_program,
	GLsizei indices_size, Cam2D *camera
) {
	glUseProgram(shader_program);
	cgui_shader_update_view(shader_program, camera);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, nullptr);
	return *this;
}

CguiRender &CguiRender::render3D(
	GLuint vao, GLuint shader_program,
	GLsizei indices_size, Cam3D *camera
) {
	glUseProgram(shader_program);
	cgui_shader_update_view3D(shader_program, camera);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, nullptr);
	return *this;
}


Mesh &Mesh::set_vertices(uint32_t row, GLfloat *vertices, uint32_t sizeof_vertices) {
	this->row_vertices = row;
	this->vertices = vertices;
	this->sizeof_vertices = sizeof_vertices;
	return *this;
}

Mesh &Mesh::set_indices(GLuint *indices, uint32_t sizeof_indices) {
	this->indices = indices;
	this->sizeof_indices = sizeof_indices;
	return *this;
}

Mesh &Mesh::add_attribute(GLint size) {
	glEnableVertexAttribArray(this->index_vertex_attribute);
	glVertexAttribPointer(
		this->index_vertex_attribute, size, GL_FLOAT, GL_FALSE,
		this->row_vertices * sizeof GLsizei,
		(void *)(this->prefix_sum_vertex_attribute * sizeof GLfloat)
	);
	this->index_vertex_attribute++;
	this->prefix_sum_vertex_attribute += size;
	return *this;
}

Mesh &Mesh::cook_vertices() {
	if (this->index_vertex_attribute) {
		cout << "index_vertex_attribute is not 0!\n" <<
			"Please use cook_vertices() first.\n";
		exit(EXIT_FAILURE);
	} else if (this->prefix_sum_vertex_attribute) {
		cout << "prefix_sum_vertex_attribute is not 0!\n" <<
			"Please use cook_vertices() first.\n";
		exit(EXIT_FAILURE);
	}
	cgui_unbindAll();
	glGenVertexArrays(1, &this->vao); // New VAO
	glBindVertexArray(this->vao); // Bind VAO

	glGenBuffers(1, &this->vbo); // New VBO
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo); // Bind VBO
	glBufferData(GL_ARRAY_BUFFER, this->sizeof_vertices, this->vertices, GL_STATIC_DRAW); // Add vertices
	// Set range of the vertices to read
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, this->row_vertices * sizeof GLfloat, nullptr);
	this->index_vertex_attribute++;
	this->prefix_sum_vertex_attribute += 3;

	glGenBuffers(1, &this->ebo); // New EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo); // Bind EBO
	// Set range of the indices to read
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->sizeof_indices, this->indices, GL_STATIC_DRAW);

	return *this;
}


// Work in progress
Cam2D &Cam2D::process() {
	this->view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, this->one_meter_size * this->scale.y));
	this->proj = glm::perspective(glm::radians(1.0f), this->aspect_ratio, 0.01f, 4000.0f);
	return *this;
}


Cam3D &Cam3D::process(
		int32_t cursor_state, Vec3i movement_input, Vec2f64 mouse_pos,
		Vec2u window_size, GLFWwindow *window
) {
	if (GLFW_CURSOR_HIDDEN == cursor_state) {
		glfwSetCursorPos(window, window_size.x / 2.0, window_size.y / 2.0);

		this->position += movement_input.x * this->speed * -glm::normalize(glm::cross(this->rotation, this->up));
		this->position += movement_input.y * this->speed * this->rotation;
		this->position += movement_input.z * this->speed * this->up;

		struct Vec2f rot = {
			this->sensitivity * ((float_t)mouse_pos.y - (window_size.y / 2)) / window_size.y,
			this->sensitivity * ((float_t)mouse_pos.x - (window_size.x / 2)) / window_size.x
		};

		glm::vec3 new_rotation = glm::rotate(
			this->rotation,
			glm::radians(-rot.x),
			glm::normalize(glm::cross(this->rotation, this->up))
		);

		if (glm::radians(88.0f) > abs(glm::angle(new_rotation, this->up) - glm::radians(90.0f))) {
			this->rotation = new_rotation;
		}
		this->rotation = glm::rotate(this->rotation, glm::radians(-rot.y), this->up);
	}
	return *this;
}


string cgui_get_file_content(string file_name) {
	std::ifstream file(file_name, std::ios::binary);

	if (!file) {
		cout << "Missing '" << file_name << "'!";
		exit(EXIT_FAILURE);
	}
	string contents;
	file.seekg(0, std::ios::end);
	contents.resize(file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(&contents[0], contents.size());
	file.close();
	return contents;
}

GLuint cgui_init_shaders(string vertex_file, string fragment_file) {
	string tmp_vertex = cgui_get_file_content(vertex_file);
	string tmp_fragment = cgui_get_file_content(fragment_file);
	const char *vertex_source = tmp_vertex.c_str();
	const char *fragment_source = tmp_fragment.c_str();

	GLint has_compiled;
	GLchar info_log[1024];
	// Compile shaders
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &has_compiled);
	if (GL_FALSE == has_compiled) {
		glGetShaderInfoLog(vertex_shader, 1024, nullptr, info_log);
		cout << "Vertex didn't compile!\n" << info_log;
	}

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_source, nullptr);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &has_compiled);
	if (GL_FALSE == has_compiled) {
		glGetShaderInfoLog(fragment_shader, 1024, nullptr, info_log);
		cout << "Fragment didn't compile!\n" << info_log;
	}

	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glGetProgramiv(shader_program, GL_LINK_STATUS, &has_compiled);
	if (GL_FALSE == has_compiled) {
		glGetShaderInfoLog(shader_program, 1024, nullptr, info_log);
		cout << "Shader program didn't link!\n" << info_log;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return shader_program;
}

void cgui_prtGLError() {
	while (GLenum err = glGetError() != GL_NO_ERROR) { cout << err << "\n"; }
}

void cgui_unbindAll() {
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void cgui_shader_update_view(GLuint shader_program, Cam2D *camera) {
	GLuint uni_model = glGetUniformLocation(shader_program, "model");
	glUniformMatrix4fv(uni_model, 1, GL_FALSE, glm::value_ptr(camera->model));
	GLuint uni_view = glGetUniformLocation(shader_program, "view");
	glUniformMatrix4fv(uni_view, 1, GL_FALSE, glm::value_ptr(camera->view));
	GLuint uni_proj = glGetUniformLocation(shader_program, "proj");
	glUniformMatrix4fv(uni_proj, 1, GL_FALSE, glm::value_ptr(camera->proj));
}

void cgui_shader_update_view3D(GLuint shader_program, Cam3D *camera) {
	glm::mat4 view = glm::lookAt(camera->position, camera->position + camera->rotation, camera->up);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), camera->aspect_ratio, 0.01f, 100.0f);

	GLuint uni_cam_matrix = glGetUniformLocation(shader_program, "camMatrix");
	glUniformMatrix4fv(uni_cam_matrix, 1, GL_FALSE, glm::value_ptr(projection * view));
}


// This is for sorting the Z-order.
// It only support unsigned.
template <typename T>
void cgui_lsd_radix_sort(vector<T> *arr) {
	int current_digit = 1;
	size_t max_value = *std::max_element(arr->begin(), arr->end());
	// It stop when there's no more digits to sort
	while (max_value / current_digit >= 1) {
		array<uint64_t, 256> prefix_sum{};
		// Counting numbers in base 256
		for (T &n : *arr) {
			prefix_sum[n / current_digit & 255]++;
		}
		// Building the prefix sum list
		for (size_t i = 1; i < 256; i++) {
			prefix_sum[i] += prefix_sum[i - 1];
		}
		// Add the elements back to sort it
		vector<T> swap(arr->size(), 0);
		for (auto iter = arr->rbegin(); ++iter != arr->rend();) {
			swap[--prefix_sum[*iter / current_digit & 255]] = *iter;
		}
		// Swapping the array with the swap
		for (size_t i = 0; i < arr->size(); i++) {
			(*arr)[i] = swap[i];
		}
		// Moving the algorithm to the next base 256 digit
		current_digit <<= 8;
	}
}
