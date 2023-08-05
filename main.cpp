#include <iostream>
using std::cout;
#include <vector>
using std::vector;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stb/stb_image.h>

#include "engine/shader_ctrl.hpp"
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
	glViewport(0, 0, win_sz.x, win_sz.y);
	// Make alpha available
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
	cgui_unbindAll();
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
	GLuint shader_program_pyramid = cgui_init_shaders("engine/shaders/pyramid.vert", "engine/shaders/pyramid.frag");
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
	cgui_unbindAll();
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO new
	GLuint VBO;
	glGenBuffers(1, &VBO); // New
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
	GLuint shader_program = cgui_init_shaders("engine/shaders/shader.vert", "engine/shaders/shader.frag");
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
	cgui_unbindAll();
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

	GLuint shader_program_image = cgui_init_shaders("engine/shaders/image.vert", "engine/shaders/image.frag");

	// Round rectangle
	GLfloat vertices3[] = {
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  -0.5f, 0.0f,
		0.5f,   0.5f, 0.0f
	};

	GLuint indices3[] = {
		0, 1, 2,
		1, 2, 3
	};

	GLuint VAO3, VBO3, EBO3;
	cgui_unbindAll();
	glGenVertexArrays(1, &VAO3);
	glBindVertexArray(VAO3);

	glGenBuffers(1, &VBO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices3, vertices3, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof GLfloat, nullptr);

	glGenBuffers(1, &EBO3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices3, indices3, GL_STATIC_DRAW);

	GLuint shader_program_rdrect = cgui_init_shaders("engine/shaders/rdrect.vert", "engine/shaders/rdrect.frag");
	glUseProgram(shader_program_rdrect);

	GLuint aColor = glGetUniformLocation(shader_program_rdrect, "aColor");
	GLuint canva_size = glGetUniformLocation(shader_program_rdrect, "aCanvaDimension");
	GLuint roundness = glGetUniformLocation(shader_program_rdrect, "aRoundness");
	glUniform3f(aColor, 0.5f, 0.5f, 1.0f);
	glUniform1f(canva_size, 1.0f);
	glUniform1f(roundness, 0.4f);

	// GPU debug
	const GLubyte *renderer = glGetString(GL_RENDERER);
	cout << renderer << "\n";

	// Init loop
	GLfloat bg_col = cgui_col8_to_f32[0x20];
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Normal

	// Camera
	struct perspective cam3D, cam2D;
	float_t aspect_ratio = (float_t)win_sz.x / (float_t)win_sz.y;
	double speed = 0.05;

	cam2D.view = glm::translate(cam2D.view, glm::vec3(0.0f, 0.0f, -2.0f));
	cam2D.proj = glm::perspective(0.0f, aspect_ratio, 0.01f, 100.0f);

	cam3D.view = glm::translate(cam3D.view, glm::vec3(0.0f, 0.0f, -2.0f));
	cam3D.proj = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.01f, 100.0f);

	cgui_prtGLError();
	cgui_unbindAll();
	while (!glfwWindowShouldClose(window)) {
		// Loop
		glfwGetFramebufferSize(window, (int*)&win_sz.x, (int*)&win_sz.y);
		glViewport(0, 0, win_sz.x, win_sz.y);
		glfwPollEvents();
		aspect_ratio = (float_t)win_sz.x / (float_t)win_sz.y;

		cam3D.view = glm::translate(cam3D.view, glm::vec3(
			(glfwGetKey(window, GLFW_KEY_A) - glfwGetKey(window, GLFW_KEY_D)) * speed,
			0.0f,
			(glfwGetKey(window, GLFW_KEY_W) - glfwGetKey(window, GLFW_KEY_S)) * speed
		));
		cam2D.proj = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.01f, 100.0f);
		cam3D.proj = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.01f, 100.0f);

		int cursor_state =
			GLFW_PRESS == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) ?
			GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL;
		glfwSetInputMode(window, GLFW_CURSOR, cursor_state);

		if (GLFW_CURSOR_HIDDEN == cursor_state) {
			glfwSetCursorPos(window, win_sz.x / 2.0, win_sz.y / 2.0);
		}


		// 3d
		glEnable(GL_DEPTH_TEST);

		glUseProgram(shader_program_pyramid);
		cgui_shader_update_view(shader_program_pyramid, &cam3D);
		glBindTexture(GL_TEXTURE_2D, brick_tex);
		glBindVertexArray(VAO4);
		glDrawElements(GL_TRIANGLES, sizeof pyramid_indices, GL_UNSIGNED_INT, nullptr);

		// We don't need depth test for 2D, we only need the Z-order for 2D.
		glDisable(GL_DEPTH_TEST);


		// 2d
		// Rainbow square
		glUseProgram(shader_program_image);
		cgui_shader_update_view(shader_program_image, &cam2D);
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, sizeof indices2, GL_UNSIGNED_INT, nullptr);

		// Triangles
		glUseProgram(shader_program);
		cgui_shader_update_view(shader_program, &cam2D);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof indices, GL_UNSIGNED_INT, nullptr);

		// Round rectangle
		glEnable(GL_BLEND);

		glUseProgram(shader_program_rdrect);
		cgui_shader_update_view(shader_program_rdrect, &cam2D);
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
