#include "shader_compiler.hpp"


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


static GLuint shader_program;
GLuint *init_shaders(string vertex_file, string fragment_file) {
	string tmp_vertex = get_file_content(vertex_file);
	string tmp_fragment = get_file_content(fragment_file);
	const char *vertex_source = tmp_vertex.c_str();
	const char *fragment_source = tmp_fragment.c_str();

	// Compile shaders
	GLint success;
	GLchar info_log[512];
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
		cout << "Vertex didn't compile!: " << info_log << "\n";
	}


	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_source, nullptr);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
		cout << "Fragment didn't compile!: " << info_log << "\n";
	}


	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return &shader_program;
}

void checkGLError() {
	while (GLenum err = glGetError()) { cout << err << "\n"; }
}
