#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/shader_compiler.hpp"
#include "engine/rgb.hpp"
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

	// VAO new
	unbindAll();
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	// VBO new
	GLuint VBO;
	glGenBuffers(1, &VBO); // New
	// VBO link to VAO
	glBindVertexArray(VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Insert vertices into VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

	// VAO new part 2
	// Define vertices slice
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof GLuint, nullptr);

	// EBO new
	GLuint EBO;
	glGenBuffers(1, &EBO); // New
	// EBO link to VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Insert indices into EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);

	// Define color slice
	glEnableVertexAttribArray(1); // This will allow you to set color.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof GLuint, (void*)(3 * sizeof GLfloat));

	// Finalize
	GLuint shader_program = init_shaders("shader/shader.vert", "shader/shader.frag");
	GLuint scale = glGetUniformLocation(shader_program, "scale");
	glUseProgram(shader_program);
	glUniform1f(scale, 0.5f);

	// Photo
	GLfloat vertices2[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f,   0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
	};

	GLuint indices2[] = {
		0, 1, 2,
		1, 2, 3
	};

	unbindAll();
	GLuint VAO2, VBO2, EBO2;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);

	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices2, vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof GLfloat, nullptr);

	glGenBuffers(1, &EBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices2, indices2, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof GLfloat, (void*)(3 * sizeof GLfloat));

	GLuint shader_program_color = init_shaders("shader/image.vert", "shader/image.frag");
	unbindAll();

	// Main loop
	GLfloat bg_col = col8_to_f32[0x20];
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Normal

	prtGLError();
	const GLubyte *renderer = glGetString(GL_RENDERER);
	cout << renderer << "\n";

	while (!glfwWindowShouldClose(window)) {
		// Init
		glfwPollEvents();

		// 1st: Back
		glUseProgram(shader_program_color);
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, sizeof indices2, GL_UNSIGNED_INT, nullptr);

		// 2nd: Front
		glUseProgram(shader_program);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof indices, GL_UNSIGNED_INT, nullptr);

		// End
		glfwSwapBuffers(window);
		glClearColor(bg_col, bg_col, bg_col, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	// Cleanup
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shader_program);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);
	glDeleteProgram(shader_program_color);
	glfwDestroyWindow(window);
	glfwTerminate();
}
