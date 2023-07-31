#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader_class.hpp"
using std::cout;


int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Init window
	GLFWwindow *window = glfwCreateWindow(800, 600, "C++ OpenGL 3.3", nullptr, nullptr);
	if (nullptr == window) {
		cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, 800, 600);

	GLuint shader_program = *init_shaders("shader.vert", "shader.frag");

	GLfloat vertices[] = {
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
	};

	GLuint indices[] = {
		0, 3, 5, // Lower left triangle
		3, 2, 4, // Upper triangle
		5, 4, 1 // Lower right triangle
	};

	// Bind array
	GLuint VAO, VBO, EBO;
	checkGLError();
	glGenVertexArrays(1, &VAO);
	checkGLError();
	glGenBuffers(1, &VBO);
	checkGLError();
	glGenBuffers(1, &EBO);
	checkGLError();
	glBindVertexArray(VAO);
	checkGLError();
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	checkGLError();
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);
	checkGLError();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	checkGLError();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);
	checkGLError();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof GLuint, nullptr);
	checkGLError();
	glEnableVertexAttribArray(0);
	checkGLError();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	checkGLError();
	glBindVertexArray(0);
	checkGLError();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	checkGLError();

	// Main loop
	glUseProgram(shader_program);
	glBindVertexArray(VAO);

	GLfloat bg_col = (1.0f / 0xff) * 0x20;
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

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
