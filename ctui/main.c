#include "src/types.h"
#include "src/gltypes.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>


int32_t main()
{	printf("Hello!\n");

	if (!glfwInit())
	{	printf("GLFW init failed.\n");
		return -1; }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	struct Vec2u win_sz = { 768, 768 };
	GLFWwindow* window = glfwCreateWindow(win_sz.x, win_sz.y, "C OpenGL 3.3", NULL, NULL);

	if (!window)
	{	printf("Failed to create GLFW window.\n");
		return -1; }

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, win_sz.x, win_sz.y);
	// Make alpha available
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Check the program is running with iGPU or dGPU.
	const GLubyte* gpu_name = glGetString(GL_RENDERER);
	printf("%s\n", gpu_name);

	// 2D
	struct Mesh rdrect;
	{	GLfloat vertices[] =
		{	-0.5f, -0.3f, 0.0f,
			-0.5f,  0.3f, 0.0f,
			0.5f, -0.3f, 0.0f,
			0.5f,  0.3f, 0.0f };
		GLuint indices[] = {0, 1, 2, 1, 2, 3 };
		rdrect.vertices = vertices;
		rdrect.indices = indices;
		rdrect.sizeof_vertices = sizeof(vertices);
		rdrect.sizeof_indices = sizeof(indices); }

	cgui_cook_vertices(&rdrect);

	// No need for a struct, they are really hard to printf.
	float model[16] =
	{	1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, -2.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	float view[16] =
	{	1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	float proj[16] =
	{	1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };

	GLuint shad_pgm_rdrect = cgui_init_shaders(
	        "src/shaders/rdrect.vert",
	        "src/shaders/rdrect.frag"
	    );
	cgui_set_uniform3f("aColor", 0.5f, 0.5f, 1.0f, shad_pgm_rdrect);
	cgui_set_uniform2f("canva_size", 1.0f, 0.6f, shad_pgm_rdrect);
	cgui_set_uniform1f("roundness", 0.4f, shad_pgm_rdrect);

	// Don't use black. It's good for hiding bugs under it.
	GLfloat bg_col = 0x20 / 256.0f;

	for (size_t i = 0; i < 16; i++)
	{	printf("%f\n", model[i]); }

	cgui_prtGLError();

	while (!glfwWindowShouldClose(window))
	{	// Input
		glfwPollEvents();
		// 2D render
		glUseProgram(shad_pgm_rdrect);
		glBindVertexArray(rdrect.vao);

		glDrawElements(GL_TRIANGLES, rdrect.sizeof_indices, GL_UNSIGNED_INT, NULL);
		// End
		glfwSwapBuffers(window);
		glClearColor(bg_col, bg_col, bg_col, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); } }
