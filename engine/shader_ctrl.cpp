﻿#include "shader_ctrl.hpp"
#include <iostream>
using std::cout;
using std::string;
#include <fstream>


string cgui_get_file_content(string file_name) {
	std::ifstream file(file_name, std::ios::binary);

	if (!file) {
		cout << "Missing '" << file_name << "'!";
		exit(-1);
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
	GLenum err = glGetError();
	if (GL_NO_ERROR != err) {
		cout << err << "\n";
	}
}

void cgui_unbindAll() {
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}


void cgui_shader_update_view(GLuint shader_program, Perspective *camera) {
	GLuint uni_model = glGetUniformLocation(shader_program, "model");
	glUniformMatrix4fv(uni_model, 1, GL_FALSE, glm::value_ptr(camera->model));
	GLuint uni_view = glGetUniformLocation(shader_program, "view");
	glUniformMatrix4fv(uni_view, 1, GL_FALSE, glm::value_ptr(camera->view));
	GLuint uni_proj = glGetUniformLocation(shader_program, "proj");
	glUniformMatrix4fv(uni_proj, 1, GL_FALSE, glm::value_ptr(camera->proj));
}


CguiRender &CguiRender::bind_texture(GLuint texture) {
	glBindTexture(GL_TEXTURE_2D, texture);
	return *this;
}

CguiRender &CguiRender::render(
	GLuint vao, GLuint shader_program,
	GLsizei indices_size, Perspective *camera
) {
	glUseProgram(shader_program);
	cgui_shader_update_view(shader_program, camera);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, nullptr);
	return *this;
}