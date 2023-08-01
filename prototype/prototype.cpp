#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_compiler.hpp"
#include <iostream>
using std::cout;


int main() {
	if (!glfwInit()) { cout << "GLFW init failed"; }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Init window
	GLFWwindow *window = glfwCreateWindow(800, 600, "C++ OpenGL 3.3", nullptr, nullptr);
	if (!window) {
		cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, 800, 600);

	GLfloat vertices[] = {
		-0.5f,    -0.5f * float(sqrt(3)) / 3,     0.0f, 0.8f, 0.3f,  0.2f,  // Lower left corner
		0.5f,     -0.5f * float(sqrt(3)) / 3,     0.0f, 0.8f, 0.3f,  0.2f,  // Lower right corner
		0.0f,      0.5f * float(sqrt(3)) * 2 / 3, 0.0f, 1.0f, 0.6f,  0.32f, // Upper corner
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6,     0.0f, 0.9f, 0.46f, 0.17f, // Inner left
		0.5f / 2,  0.5f * float(sqrt(3)) / 6,     0.0f, 0.9f, 0.46f, 0.17f, // Inner right
		0.0f,     -0.5f * float(sqrt(3)) / 3,     0.0f, 0.8f, 0.3f,  0.02f  // Inner down
	};

	GLuint indices[] = {
		0, 3, 5, // Lower left triangle
		3, 2, 4, // Upper triangle
		5, 4, 1 // Lower right triangle
	};

	GLuint VAO, VBO, EBO;
	// VAO new
	glGenVertexArrays(1, &VAO);

	// VBO bind
	glGenBuffers(1, &VBO);
	glBindVertexArray(VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

	// VAO new part 2
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof GLuint, nullptr);
	glEnableVertexAttribArray(VAO);

	// EBO new
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);

	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof GLuint, (void*)(3 * sizeof GLfloat));

	// Unbind
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Main loop
	GLfloat bg_col = (1.0f / 0xff) * 0x20;

	GLuint shader_program = *init_shaders("shader.vert", "shader.frag");
	glUseProgram(shader_program);
	glBindVertexArray(VAO);
	GLuint scale = glGetUniformLocation(shader_program, "scale");
	glUniform1f(scale, 0.5f);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glDrawElements(GL_TRIANGLES, 3 * sizeof GLuint, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glClearColor(bg_col, bg_col, bg_col, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	// Cleanup
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shader_program);
	glfwDestroyWindow(window);
	glfwTerminate();
}
