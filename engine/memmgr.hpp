#pragma once
#include <vector>


// Watch out for some GLuint pointer that has GLsizei more than 1.
// Because the pool doesn't support removing it.
// You don't have to worrying about dangling pointers in this.
// Because GLFW doesn't crash with dangling pointers.
class PointerPoolGL {
private:
	std::vector<GLuint>
		VAOs{}, VBOs{}, EBOs{},
		shader_programs{}, textures{};

public:
	PointerPoolGL &vao(GLuint vao) {
		VAOs.push_back(vao);
		return *this;
	}
	PointerPoolGL &vbo(GLuint vbo) {
		VBOs.push_back(vbo);
		return *this;
	}
	PointerPoolGL &ebo(GLuint ebo) {
		EBOs.push_back(ebo);
		return *this;
	}
	PointerPoolGL &sha_pgm(GLuint shader_program) {
		shader_programs.push_back(shader_program);
		return *this;
	}
	PointerPoolGL &texture(GLuint texture) {
		textures.push_back(texture);
		return *this;
	}

	void free() {
		for (size_t i = 0; i < VAOs.size(); i++) {
			glDeleteVertexArrays(1, &VAOs[i]);
			glDeleteBuffers(1, &VBOs[i]);
			glDeleteBuffers(1, &EBOs[i]);
		}
		for (size_t i = 0; i < shader_programs.size(); i++) {
			glDeleteProgram(shader_programs[i]);
		}
		for (size_t i = 0; i < textures.size(); i++) {
			glDeleteTextures(1, &textures[i]);
		}
	}
};
