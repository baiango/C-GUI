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

	struct Vec2u win_sz = {768, 768 };
	GLFWwindow* window = glfwCreateWindow(win_sz.x, win_sz.y, "C OpenGL 3.3", NULL, NULL);

	if (!window)
	{	printf("Failed to create GLFW window.\n");
		return -1; }

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, win_sz.x, win_sz.y);
	// Make alpha available
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

	float model[] =
	{	1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, -2.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	float view[] =
	{	1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	float proj[] =
	{	1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };

	char* file_content = cgui_read_file();

	printf("File content:\n%s", file_content);

	// Don't use black. It's good for hiding bugs under it.
	GLfloat bg_col = 0x20 / 256.0f;

	while (!glfwWindowShouldClose(window))
	{	glfwPollEvents();

		glfwSwapBuffers(window);
		glClearColor(bg_col, bg_col, bg_col, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); } }
