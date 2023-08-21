#include "src/gllib/import.h"

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
		{	-0.5f, -0.3f, -0.1f,
			-0.5f,  0.3f, -0.1f,
			0.5f,  -0.3f, -0.1f,
			0.5f,   0.3f, -0.1f };
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

	struct Vec3f position;
	struct Vec3f rotation;
	struct Vec3f up;
	cgui_set_vec3f_from_floats(&position, 1.0f, -2.0f, -1.0f);
	cgui_set_vec3f_from_floats(&rotation, 0.0f, 0.0f, -1.0f);
	cgui_set_vec3f_from_floats(&up, 0.0f, 1.0f, 0.0f);

	struct Vec3f tmp;
	cgui_add_vec3f(&tmp, &position, &rotation);
	cgui_lookat(&view, &position, &tmp, &up);

	cgui_perspective(&proj, 45.0f * CGUI_ONE_DEG_IN_RAD, 1.0f, 0.01f, 100.0f);

	struct mat4 view_proj_mat;
	cgui_zero_mat4(&view_proj_mat);
	cgui_mul_mat4(&view_proj_mat, &view, &proj);
	
	struct Vec3f camera_position;
	struct Vec3f camera_target;
	struct Vec3f camera_up;
	cgui_set_vec3f_from_floats(&camera_position, 0.0f, 0.0f, -1.0f);
	cgui_set_vec3f_from_floats(&camera_target, 0.0f, 0.0f, 0.0f);
	cgui_set_vec3f_from_floats(&camera_up, 0.0f, 1.0f, 0.0f);
	struct mat4 view2;
	cgui_lookat(&view2, &camera_position, &camera_target, &camera_up);

	float left = -0.5f; // Half a meter from middle to the left
	float right = 0.5f; // Half a meter from middle to the right
	float bottom = -0.5; // 16:9 aspect ratio, so -0.5 * 9 / 16 = -0.28125
	float top = 0.5; // 16:9 aspect ratio, so 0.5 * 9 / 16 = 0.28125
	float near = 0.01f; // Anything closer than this will clip.
	float far = 4000.0f; // Anything further than this will clip.
	struct mat4 ortho_proj;
	cgui_ortho(&ortho_proj, left, right, bottom, top, near, far);
	
	struct mat4 ortho_view_proj;
	cgui_mul_mat4(&ortho_view_proj, &ortho_proj, &view2);

	cgui_prt_mat4(&view2);
	printf("\n");
	cgui_prt_mat4(&ortho_proj);
	printf("\n");
	cgui_prt_mat4(&ortho_view_proj);
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

		GLuint uni_cam_matrix = glGetUniformLocation(shad_pgm_rdrect, "camMatrix");
		glUniformMatrix4fv(uni_cam_matrix, 1, GL_FALSE, ortho_view_proj.data);

		glBindVertexArray(rdrect.vao);
		glDrawElements(GL_TRIANGLES, rdrect.sizeof_indices, GL_UNSIGNED_INT, NULL);
		glDisable(GL_BLEND);
		// End
		glfwSwapBuffers(window);
		glClearColor(bg_col, bg_col, bg_col, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); } }
