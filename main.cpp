#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "cgui/engine.hpp"
#include "cgui/rgb.hpp"
#include "cgui/types.hpp"
#include "cgui/memmgr.hpp"

#include <iostream>
using std::cout;


// This will become test unit "scene0".
int32_t main() {
	if (!glfwInit()) {
		cout << "GLFW init failed.";
		return EXIT_FAILURE;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Init window
	struct Vec2u win_sz = {768, 768};
	GLFWwindow *window = glfwCreateWindow(win_sz.x, win_sz.y, "C++ OpenGL 3.3", nullptr, nullptr);
	if (!window) {
		cout << "Failed to create GLFW window.\n";
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // VSync, doesn't work.
	gladLoadGL();
	glViewport(0, 0, win_sz.x, win_sz.y);
	// Make alpha available
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// C-GUI
	class PointerPoolGL defer_ptr_pool;
	class CguiRender canva_renderer;

	// 3D
	class Mesh pyramid;
	{
		GLfloat pyramid_vertices[] = {
			// Pos / Color / TexCoord //
			-0.5f, 0.0f,  0.5f,   0.83f, 0.70f, 0.44f,   0.0f, 0.0f,
			-0.5f, 0.0f, -0.5f,   0.83f, 0.70f, 0.44f,   5.0f, 0.0f,
			0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,   0.0f, 0.0f,
			0.5f, 0.0f,  0.5f,    0.83f, 0.70f, 0.44f,   5.0f, 0.0f,
			0.0f, 0.8f,  0.0f,    0.92f, 0.86f, 0.76f,   2.5f, 5.0f
		};

		// Indices for vertices order
		GLuint pyramid_indices[] = {
			0, 1, 2, 0, 2, 3,
			0, 1, 4, 1, 2, 4,
			2, 3, 4, 3, 0, 4
		};

		// VAO new
		pyramid
			.set_vertices(8, pyramid_vertices, sizeof pyramid_vertices)
			.set_indices(pyramid_indices, sizeof pyramid_indices)
			.cook_vertices()
			.add_attribute(3) // Color
			.add_attribute(2); // UV
	}
	// Finalize
	// Texture
	GLuint brick_tex;
	{
		glGenTextures(1, &brick_tex);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, brick_tex);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		struct Vec2i img_sz {};
		int32_t img_col_ch;
		stbi_set_flip_vertically_on_load(true);
		uint8_t *brick_bytes = stbi_load("cgui/image/brick.png", &img_sz.x, &img_sz.y, &img_col_ch, 0);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_sz.x, img_sz.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, brick_bytes);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(brick_bytes);
	}
	// Shader program
	GLuint shader_program_pyramid = cgui_init_shaders("cgui/shaders/pyramid.vert", "cgui/shaders/pyramid.frag");
	glUseProgram(shader_program_pyramid);
	GLuint tex0 = glGetUniformLocation(shader_program_pyramid, "tex0");
	glUniform1i(tex0, 0);

	// Defer
	defer_ptr_pool
		.vao(pyramid.vao).vbo(pyramid.vbo).ebo(pyramid.ebo)
		.sha_pgm(shader_program_pyramid)
		.texture(brick_tex);

	class Cvector<int> aaa;
	aaa.init(100);

	// 2D
	class Mesh triangles;
	{
		GLfloat vertices[] = {
			// Pos / Color //
			-0.5f,    -0.5f * float(sqrt(3)) / 3,     0.0f,   0.8f, 0.3f,  0.2f,  // Lower left corner
			0.5f,     -0.5f * float(sqrt(3)) / 3,     0.0f,   0.8f, 0.3f,  0.2f,  // Lower right corner
			0.0f,      0.5f * float(sqrt(3)) * 2 / 3, 0.0f,   1.0f, 0.6f,  0.32f, // Upper corner
			-0.5f / 2, 0.5f * float(sqrt(3)) / 6,     0.0f,   0.9f, 0.46f, 0.17f, // Inner left
			0.5f / 2,  0.5f * float(sqrt(3)) / 6,     0.0f,   0.9f, 0.46f, 0.17f, // Inner right
			0.0f,     -0.5f * float(sqrt(3)) / 3,     0.0f,   0.8f, 0.3f,  0.02f  // Inner down
		};

		GLuint indices[] = {
			0, 3, 5, // Lower left triangle
			3, 2, 4, // Upper triangle
			5, 4, 1 // Lower right triangle
		};

		// VAO new
		triangles
			.set_vertices(6, vertices, sizeof vertices)
			.set_indices(indices, sizeof indices)
			.cook_vertices()
			.add_attribute(3); // Color slice
	}
	// Finalize
	GLuint shader_program = cgui_init_shaders("cgui/shaders/shader.vert", "cgui/shaders/shader.frag");
	GLuint scale = glGetUniformLocation(shader_program, "scale");
	glUseProgram(shader_program);
	glUniform1f(scale, 0.5f);

	// Defer free
	defer_ptr_pool
		.vao(triangles.vao).vbo(triangles.vbo).ebo(triangles.ebo)
		.sha_pgm(shader_program);

	// Photo
	class Mesh rainbow_square;
	{
		GLfloat vertices2[] = {
			// Pos / Color //
			-1.0f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
			-1.0f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
			0.0f,  -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
			0.0f,   0.5f, 0.0f,   1.0f, 0.0f, 1.0f,
		};

		GLuint indices2[] = {
			0, 1, 2, 1, 2, 3
		};

		rainbow_square
			.set_vertices(6, vertices2, sizeof vertices2)
			.set_indices(indices2, sizeof indices2)
			.cook_vertices()
			.add_attribute(3); // Color
	}
	GLuint shader_program_image = cgui_init_shaders("cgui/shaders/image.vert", "cgui/shaders/image.frag");

	defer_ptr_pool
		.vao(rainbow_square.vao).vbo(rainbow_square.vbo).ebo(rainbow_square.ebo)
		.sha_pgm(shader_program_image);

	// Round rectangle
	class Mesh rdrect;
	{
		GLfloat vertices3[] = {
			// Pos //
			-0.5f, -0.3f, 0.0f,
			-0.5f,  0.3f, 0.0f,
			0.5f,  -0.3f, 0.0f,
			0.5f,   0.3f, 0.0f
		};

		GLuint indices3[] = {
			0, 1, 2, 1, 2, 3
		};

		rdrect
			.set_vertices(3, vertices3, sizeof vertices3)
			.set_indices(indices3, sizeof indices3)
			.cook_vertices();
	}
	GLuint shader_program_rdrect = cgui_init_shaders("cgui/shaders/rdrect.vert", "cgui/shaders/rdrect.frag");
	glUseProgram(shader_program_rdrect);

	GLuint aColor = glGetUniformLocation(shader_program_rdrect, "aColor");
	GLuint canva_size = glGetUniformLocation(shader_program_rdrect, "aCanvaDimension");
	GLuint roundness = glGetUniformLocation(shader_program_rdrect, "aRoundness");
	glUniform3f(aColor, 0.5f, 0.5f, 1.0f);
	glUniform2f(canva_size, 1.0f, 0.6f);
	glUniform1f(roundness, 0.4f);

	defer_ptr_pool
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
	class Cam2D cam2D;
	class Cam3D cam3D;
	float_t aspect_ratio = (float_t)win_sz.x / (float_t)win_sz.y;

	cam3D.position = glm::vec3(0.0f, 0.0f, 1.5f);

	cgui_prtGLError();
	cgui_unbindAll();
	while (!glfwWindowShouldClose(window)) {
		// Input
		glfwPollEvents();
		// Mouse
		struct Vec2f64 mouse_pos {};
		glfwGetCursorPos(window, &mouse_pos.x, &mouse_pos.y);

		// Keyboard
		struct Vec3i movement_input = { // Z is up
			glfwGetKey(window, GLFW_KEY_A) - glfwGetKey(window, GLFW_KEY_D),
			glfwGetKey(window, GLFW_KEY_W) - glfwGetKey(window, GLFW_KEY_S),
			glfwGetKey(window, GLFW_KEY_SPACE) - glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)
		};

		// Process
		// Mouse
		int32_t cursor_state =
			GLFW_PRESS == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) ?
			GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL;
		glfwSetInputMode(window, GLFW_CURSOR, cursor_state);

		// Window resize
		glfwGetFramebufferSize(window, (int32_t *)&win_sz.x, (int32_t *)&win_sz.y);
		glViewport(0, 0, win_sz.x, win_sz.y);
		aspect_ratio = (float_t)win_sz.x / (float_t)win_sz.y;
		cam2D.aspect_ratio = aspect_ratio;
		cam3D.aspect_ratio = aspect_ratio;

		cam3D.process(
			cursor_state, movement_input, mouse_pos,
			win_sz, window
		);
		cam2D.process(); // Work in progress

		// 3D render
		glEnable(GL_DEPTH_TEST);
		canva_renderer
			.bind_texture(brick_tex)
			.render3D(pyramid.vao, shader_program_pyramid, pyramid.sizeof_indices, &cam3D);
		glDisable(GL_DEPTH_TEST); // We don't need depth test for 2D, we only need the Z-order for 2D.

		// 2D render
		canva_renderer
			.render(rainbow_square.vao, shader_program_image, rainbow_square.sizeof_indices, &cam2D);
		canva_renderer
			.render(triangles.vao, shader_program, triangles.sizeof_indices, &cam2D);
		// With alpha
		glEnable(GL_BLEND);
		canva_renderer
			.render(rdrect.vao, shader_program_rdrect, rdrect.sizeof_indices, &cam2D);
		glDisable(GL_BLEND);

		// End
		glfwSwapBuffers(window);
		glClearColor(bg_col, bg_col, bg_col, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// Cleanup
	defer_ptr_pool.free();
	glfwDestroyWindow(window);
	glfwTerminate();
}
