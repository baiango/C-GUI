#include "shader_class.hpp"


string get_file_content(string file_name) {
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

GLuint *init_shaders(string vertex_file, string fragment_file) {
	string tmp_vertex = get_file_content(vertex_file);
	string tmp_fragment = get_file_content(fragment_file);
	const char *vertex_source = tmp_vertex.c_str();
	const char *fragment_source = tmp_fragment.c_str();

	// Compile shaders
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	checkGLError();
	glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
	checkGLError();
	glCompileShader(vertex_shader);
	checkGLError();

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	checkGLError();
	glShaderSource(fragment_shader, 1, &fragment_source, nullptr);
	checkGLError();
	glCompileShader(fragment_shader);
	checkGLError();

	GLuint shader_program = glCreateProgram();
	checkGLError();
	glAttachShader(shader_program, vertex_shader);
	checkGLError();
	glAttachShader(shader_program, fragment_shader);
	checkGLError();
	glLinkProgram(shader_program);
	checkGLError();

	glDeleteShader(vertex_shader);
	checkGLError();
	glDeleteShader(fragment_shader);
	checkGLError();

	return &shader_program;
}

void checkGLError()
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << err;
	}
}