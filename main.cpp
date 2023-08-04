#include <iostream>
using std::cout;
#include <vector>
using std::vector;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stb/stb_image.h>

#include "engine/shader_controller.hpp"
#include "engine/rgb.hpp"
#include "engine/types.hpp"


int main() {
	if (!glfwInit()) { cout << "GLFW init failed"; }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Init window
	const struct Vec2u win_sz = {768, 768};
	GLFWwindow *window = glfwCreateWindow(win_sz.x, win_sz.y, "C++ OpenGL 3.3", nullptr, nullptr);
	if (!window) {
		cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Vsync
	gladLoadGL();
	// Make alpha available
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, win_sz.x, win_sz.y);

	// 3D
	GLfloat pyramid_vertices[] = {
		-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
		0.5f, 0.0f, -0.5f,      0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
		0.5f, 0.0f,  0.5f,      0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
		0.0f, 0.8f,  0.0f,      0.92f, 0.86f, 0.76f,	2.5f, 5.0f
	};

	// Indices for vertices order
	GLuint pyramid_indices[] = {
		0, 1, 2, 0, 2, 3,
		0, 1, 4, 1, 2, 4,
		2, 3, 4, 3, 0, 4
	};

	// VAO new
	GLuint VAO4, VBO4, EBO4;
	unbindAll();
	glGenVertexArrays(1, &VAO4);
	glBindVertexArray(VAO4);

	glGenBuffers(1, &VBO4);
	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof pyramid_vertices, pyramid_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof GLuint, nullptr);

	glGenBuffers(1, &EBO4);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof pyramid_indices, pyramid_indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof GLuint, (void*)(3 * sizeof GLfloat));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof GLuint, (void*)(6 * sizeof GLfloat));

	// Finalize
	// Texture
	GLuint brick_tex;
	glGenTextures(1, &brick_tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brick_tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	Vec2i img_sz;
	int32_t img_col_ch;
	stbi_set_flip_vertically_on_load(true);
	uint8_t *brick_bytes = stbi_load("engine/image/brick.png", &img_sz.x, &img_sz.y, &img_col_ch, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_sz.x, img_sz.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, brick_bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(brick_bytes);

	// Shader program
	GLuint shader_program_pyramid = init_shaders("engine/shaders/pyramid.vert", "engine/shaders/pyramid.frag");
	glUseProgram(shader_program_pyramid);
	GLuint tex0 = glGetUniformLocation(shader_program_pyramid, "tex0");
	glUniform1i(tex0, 0);

	// 2D
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
	glBindVertexArray(VAO);

	// VBO new
	GLuint VBO;
	glGenBuffers(1, &VBO); // New
	prtGLError();
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
	GLuint shader_program = init_shaders("engine/shaders/shader.vert", "engine/shaders/shader.frag");
	GLuint scale = glGetUniformLocation(shader_program, "scale");
	glUseProgram(shader_program);
	glUniform1f(scale, 0.5f);

	// Photo
	GLfloat vertices2[] = {
		-1.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-1.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.0f,   0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
	};

	GLuint indices2[] = {
		0, 1, 2,
		1, 2, 3
	};

	GLuint VAO2, VBO2, EBO2;
	unbindAll();
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

	GLuint shader_program_image = init_shaders("engine/shaders/image.vert", "engine/shaders/image.frag");

	// Round rectangle
	GLfloat vertices3[] = {
		-0.3f, -0.3f, 0.0f,
		-0.3f,  0.3f, 0.0f,
		0.3f,  -0.3f, 0.0f,
		0.3f,   0.3f, 0.0f
	};

	GLuint indices3[] = {
		0, 1, 2,
		1, 2, 3
	};

	GLuint VAO3, VBO3, EBO3;
	unbindAll();
	glGenVertexArrays(1, &VAO3);
	glBindVertexArray(VAO3);

	glGenBuffers(1, &VBO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices3, vertices3, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof GLfloat, nullptr);

	glGenBuffers(1, &EBO3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices3, indices3, GL_STATIC_DRAW);

	GLuint shader_program_rdrect = init_shaders("engine/shaders/rdrect.vert", "engine/shaders/rdrect.frag");
	glUseProgram(shader_program_rdrect);
	GLuint aColor = glGetUniformLocation(shader_program_rdrect, "aColor");
	glUniform3f(aColor, 0.5f, 0.5f, 1.0f);

	// GPU debug
	//prtGLError();
	const GLubyte *renderer = glGetString(GL_RENDERER);
	cout << renderer << "\n";

	// Main loop
	GLfloat bg_col = col8_to_f32[0x20];
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Normal
	float rotation = 0.0f;
	double previous_time = glfwGetTime();

	unbindAll();
	struct Vec2i win_sz2 = {0, 0};
	while (!glfwWindowShouldClose(window)) {
		// Init
		glfwGetFramebufferSize(window, (int*)&win_sz.x, (int*)&win_sz.y);
		glViewport(0, 0, win_sz.x, win_sz.y);
		glfwPollEvents();
		struct perspective cam;
		cam.model = glm::rotate(cam.model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		cam.view = glm::translate(cam.view, glm::vec3(0.0f, 0.0f, -2.0f));
		cam.proj = glm::perspective(
			glm::radians(45.0f), (float)win_sz.x / (float)win_sz.y, 0.01f, 100.0f
		);

		struct perspective cam_2D;
		cam_2D.model = glm::rotate(cam_2D.model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		cam_2D.view = glm::translate(cam_2D.view, glm::vec3(0.0f, 0.0f, -2.0f));
		cam_2D.proj = glm::perspective(
			glm::radians(45.0f), (float)win_sz.x / (float)win_sz.y, 0.01f, 100.0f
		);

		double current_time =	glfwGetTime();
		if (1.0 / 60 < current_time - previous_time) {
			rotation += 0.5f;
			previous_time = current_time;
		}

		// 3d
		glEnable(GL_DEPTH_TEST);

		glUseProgram(shader_program_pyramid);
		update_view(shader_program_pyramid, &cam);
		glBindTexture(GL_TEXTURE_2D, brick_tex);
		glBindVertexArray(VAO4);
		glDrawElements(GL_TRIANGLES, sizeof pyramid_indices, GL_UNSIGNED_INT, nullptr);

		glDisable(GL_DEPTH_TEST);

		// 2d
		// Rainbow square
		glUseProgram(shader_program_image);
		update_view(shader_program_image, &cam_2D);
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, sizeof indices2, GL_UNSIGNED_INT, nullptr);

		// Triangles
		glUseProgram(shader_program);
		update_view(shader_program, &cam_2D);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof indices, GL_UNSIGNED_INT, nullptr);

		// Round rectangle
		glEnable(GL_BLEND);
		glUseProgram(shader_program_rdrect);
		update_view(shader_program_rdrect, &cam_2D);
		glBindVertexArray(VAO3);
		glDrawElements(GL_TRIANGLES, sizeof indices3, GL_UNSIGNED_INT, nullptr);
		glDisable(GL_BLEND);

		// End
		glfwSwapBuffers(window);
		glClearColor(bg_col, bg_col, bg_col, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// Cleanup
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shader_program);

	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);
	glDeleteProgram(shader_program_image);

	glDeleteVertexArrays(1, &VAO3);
	glDeleteBuffers(1, &VBO3);
	glDeleteBuffers(1, &EBO3);
	glDeleteProgram(shader_program_rdrect);

	glDeleteVertexArrays(1, &VAO4);
	glDeleteBuffers(1, &VBO4);
	glDeleteBuffers(1, &EBO4);
	glDeleteProgram(shader_program_pyramid);
	glDeleteTextures(1, &brick_tex);

	glfwDestroyWindow(window);
	glfwTerminate();
}
