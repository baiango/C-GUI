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
#include "engine/memmgr.hpp"


int32_t main() {
	if (!glfwInit()) { cout << "GLFW init failed."; }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Init window
	struct vec2u win_sz = {768, 768};
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

	// C-GUI
	class PointerPoolGL ptr_pool;
	class CguiRender canva_renderer;

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
	Mesh pyramid;
	pyramid
		.set_vertices(8, pyramid_vertices, sizeof pyramid_vertices)
		.set_indices(pyramid_indices, sizeof pyramid_indices)
		.cook_vertices();

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof GLuint, (void *)(3 * sizeof GLfloat));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof GLuint, (void *)(6 * sizeof GLfloat));

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

	vec2i img_sz;
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

	// Defer
	ptr_pool.vao(pyramid.vao).vbo(pyramid.vbo).ebo(pyramid.ebo)
		.sha_pgm(shader_program_pyramid)
		.texture(brick_tex);

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
	Mesh triangles;
	triangles
		.set_vertices(6, vertices, sizeof vertices)
		.set_indices(indices, sizeof indices)
		.cook_vertices();

	// Define color slice
	glEnableVertexAttribArray(1); // This will allow you to set color.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof GLuint, (void *)(3 * sizeof GLfloat));

	// Finalize
	GLuint shader_program = cgui_init_shaders("engine/shaders/shader.vert", "engine/shaders/shader.frag");
	GLuint scale = glGetUniformLocation(shader_program, "scale");
	glUseProgram(shader_program);
	glUniform1f(scale, 0.5f);

	// Defer free
	ptr_pool
		.vao(triangles.vao).vbo(triangles.vbo).ebo(triangles.ebo)
		.sha_pgm(shader_program);

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

	struct Mesh mesh2;
	mesh2
		.set_vertices(6, vertices2, sizeof vertices2)
		.set_indices(indices2, sizeof indices2)
		.cook_vertices();

	glEnableVertexAttribArray(1); // Unlock new vertex attribute
	// Set color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof GLfloat, (void *)(3 * sizeof GLfloat));

	GLuint shader_program_image = cgui_init_shaders("engine/shaders/image.vert", "engine/shaders/image.frag");

	ptr_pool
		.vao(mesh2.vao).vbo(mesh2.vbo).ebo(mesh2.ebo)
		.sha_pgm(shader_program_image);

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

	Mesh rdrect;
	rdrect
		.set_vertices(3, vertices3, sizeof vertices3)
		.set_indices(indices3, sizeof indices3)
		.cook_vertices();

	GLuint shader_program_rdrect = cgui_init_shaders("engine/shaders/rdrect.vert", "engine/shaders/rdrect.frag");
	glUseProgram(shader_program_rdrect);

	GLuint aColor = glGetUniformLocation(shader_program_rdrect, "aColor");
	GLuint canva_size = glGetUniformLocation(shader_program_rdrect, "aCanvaDimension");
	GLuint roundness = glGetUniformLocation(shader_program_rdrect, "aRoundness");
	glUniform3f(aColor, 0.5f, 0.5f, 1.0f);
	glUniform1f(canva_size, 1.0f);
	glUniform1f(roundness, 0.4f);

	ptr_pool
		.vao(rdrect.vao).vbo(rdrect.vbo).ebo(rdrect.ebo)
		.sha_pgm(shader_program_rdrect);

	// GPU debug
	const GLubyte *gpu_name = glGetString(GL_RENDERER);
	cout << gpu_name << "\n";

	// Init loop
	GLfloat bg_col = cgui_col8_to_f32[0x20];
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Normal

	// Camera
	struct Perspective cam2D, cam3D;
	struct vec2f32 cam2D_scale = {2.0f, 2.0f};
	float_t aspect_ratio = (float_t)win_sz.x / (float_t)win_sz.y;
	double_t speed = 0.05;

	cam2D.view = glm::translate(cam2D.view, glm::vec3(0.0f, 0.0f, -2.0f));
	cam3D.view = glm::translate(cam3D.view, glm::vec3(0.0f, 0.0f, -2.0f));
	cam3D.proj = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.01f, 100.0f);

	cgui_prtGLError();
	cgui_unbindAll();

	while (!glfwWindowShouldClose(window)) {
		// Input
		glfwPollEvents();
		int32_t cursor_state =
			GLFW_PRESS == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) ?
			GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL;
		glfwSetInputMode(window, GLFW_CURSOR, cursor_state);

		struct vec2i movement_input = {
			glfwGetKey(window, GLFW_KEY_A) - glfwGetKey(window, GLFW_KEY_D),
			glfwGetKey(window, GLFW_KEY_W) - glfwGetKey(window, GLFW_KEY_S)
		};

		if (GLFW_CURSOR_HIDDEN == cursor_state) {
			glfwSetCursorPos(window, win_sz.x / 2.0, win_sz.y / 2.0);
		}

		// Window resize
		glfwGetFramebufferSize(window, (int32_t *)&win_sz.x, (int32_t *)&win_sz.y);
		glViewport(0, 0, win_sz.x, win_sz.y);
		aspect_ratio = (float_t)win_sz.x / (float_t)win_sz.y;

		cam2D.view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -57.25f * cam2D_scale.y));
		cam2D.proj = glm::perspective(glm::radians(1.0f), aspect_ratio, 0.01f, 4000.0f);

		cam3D.view = glm::translate(cam3D.view, glm::vec3(
			movement_input.x * speed, 0.0f, movement_input.y * speed
		));
		cam3D.proj = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.01f, 100.0f);


		// 3D render
		glEnable(GL_DEPTH_TEST);

		// Pyramid
		canva_renderer
			.bind_texture(brick_tex)
			.render(pyramid.vao, shader_program_pyramid, sizeof pyramid_indices, &cam3D);

		// We don't need depth test for 2D, we only need the Z-order for 2D.
		glDisable(GL_DEPTH_TEST);


		// 2D render
		// Rainbow square
		canva_renderer
			.render(mesh2.vao, shader_program_image, sizeof indices2, &cam2D);

		// Triangles
		canva_renderer
			.render(triangles.vao, shader_program, sizeof indices, &cam2D);

		// Enable alpha
		glEnable(GL_BLEND);

		// Round rectangle
		canva_renderer
			.render(rdrect.vao, shader_program_rdrect, sizeof indices3, &cam2D);

		glDisable(GL_BLEND);

		// End
		glfwSwapBuffers(window);
		glClearColor(bg_col, bg_col, bg_col, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// Cleanup
	ptr_pool.free();
	glfwDestroyWindow(window);
	glfwTerminate();
}
