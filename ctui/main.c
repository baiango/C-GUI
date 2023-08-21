#include "src/types.h"
#include "src/gltypes.h"
#include "src/glmath.h"

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
		rdrect.sizeof_indices = sizeof(indices);
		rdrect.row = 3;
		rdrect.index_attribute = 0;
		rdrect.prefix_sum_attribute = 0; }

	cgui_cook_vertices(&rdrect);

	GLuint shad_pgm_rdrect = cgui_init_shaders(
	        "src/shaders/rdrect.vert",
	        "src/shaders/rdrect.frag"
	    );
	cgui_set_uniform3f("aColor", 0.5f, 0.5f, 1.0f, shad_pgm_rdrect);
	cgui_set_uniform2f("aCanvaDimension", 1.0f, 0.6f, shad_pgm_rdrect);
	cgui_set_uniform1f("aRoundness", 0.4f, shad_pgm_rdrect);

	struct mat4 view;
	cgui_set_mat4(&view,
	    1.0f, 0.0f, 0.0f, 0.0f,
	    0.0f, 1.0f, 0.0f, 0.0f,
	    0.0f, 0.0f, 1.0f, 0.0f,
	    0.0f, 0.0f, 0.0f, 1.0f );
	struct mat4 proj;
	cgui_set_mat4(&proj,
	    1.0f, 0.0f, 0.0f, 0.0f,
	    0.0f, 1.0f, 0.0f, 0.0f,
	    0.0f, 0.0f, 1.0f, 0.0f,
	    0.0f, 0.0f, 0.0f, 1.0f );

	struct vec3f position;
	struct vec3f rotation;
	struct vec3f up;
	cgui_set_vec3f_from_floats(&position, 1.0f, -2.0f, 0.0f);
	cgui_set_vec3f_from_floats(&rotation, 0.0f, 0.0f, -1.0f);
	cgui_set_vec3f_from_floats(&up, 0.0f, 1.0f, 0.0f);

	struct vec3f tmp;
	cgui_add_vec3f(&tmp, &position, &rotation);
	cgui_lookat(&view, &position, &tmp, &up);
	cgui_prt_mat4(&view);
	printf("\n");

	cgui_perspective(&proj, 45.0f * CGUI_ONE_DEG_IN_RAD, 1.0f, 0.01f, 100.0f);
	cgui_prt_mat4(&proj);
	printf("\n");

	struct mat4 view_proj_mat;
	cgui_zero_mat4(&view_proj_mat);
	cgui_mul_mat4(&view_proj_mat, &view, &proj);
	cgui_prt_mat4(&view_proj_mat);
	printf("\n");

	cgui_prt_GLError();

	// Don't use black. It's good for hiding bugs under it.
	GLfloat bg_col = 0x20 / 256.0f;

	while (!glfwWindowShouldClose(window))
	{	// Input
		glfwPollEvents();
		// 2D render
		glEnable(GL_BLEND);
		glUseProgram(shad_pgm_rdrect);
		glBindVertexArray(rdrect.vao);

		GLuint uni_cam_matrix = glGetUniformLocation(shad_pgm_rdrect, "camMatrix");
		glUniformMatrix4fv(uni_cam_matrix, 1, GL_FALSE, view_proj_mat.data);
		glDrawElements(GL_TRIANGLES, rdrect.sizeof_indices, GL_UNSIGNED_INT, NULL);
		glDisable(GL_BLEND);
		// End
		glfwSwapBuffers(window);
		glClearColor(bg_col, bg_col, bg_col, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); } }
