#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>


struct Mesh
{	uint16_t row;
	GLfloat* vertices;
	GLuint* indices;
	uint16_t sizeof_vertices;
	uint16_t sizeof_indices;
	GLuint vao, vbo, ebo, shader_program;
	uint16_t index_attribute;
	uint16_t prefix_sum_attribute; };


struct PointerPoolGL
{	GLuint* VAOs, * VBOs, * EBOs;
	GLuint* shader_programs; };


void cgui_unbindAll()
{	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0); }


// To get meshes working we need do these thing first.
// (This is not completely correct yet. As I don't fully understands matrix yet.)
// 1.	Define your vertices and indices.
// 2.	Make new VAO and bind it.
// 3.	Make new VBO and bind it.
// 4.	Then add vertices to VBO.
// 5.	Set range of the vertices to read using glVertexAttribPointer().
// 6.	Make new EBO and bind it.
// 7.	Set range of the indices to read using glBufferData()
//		(Optionally)
// 8.	Maybe unbind it make sure other mesh won't use it.
//		(I guard it with cgui_unbindAll(). So I don't have worry about it.)
// 9.	Maybe enable more attribute using glEnableVertexAttribArray().
// 10.	Add more attribute to send it to the vertex shader using glVertexAttribPointer().
//		Example. vertex color, scale, and position.
//		or you can just use glUniform1f() or inside the fragment with keyword "uniform" to define solid color and save memory on the vertex array.
// 11.	Transform the mesh's position with matrix4.
//		| sx 0  0  px |
//		| 0  sy 0  py |
//		| 0  0  sz pz |
//		"p" is position, "s" is scale, and "r" is rotation.
//		| 1 0        0       0 |
//		| 0 cos(rx) -sin(rx) 0 |
//		| 0 sin(rx)  cos(rx) 0 |
//
//		|  cos(ry) 0 sin(ry) 0 |
//		|  0       1 0       0 |
//		| -sin(ry) 0 cos(ry) 0 |
//
//		| cos(rz) -sin(rz) 0 0 |
//		| sin(rz)  cos(rz) 0 0 |
//		| 0        0       1 0 |
//		Angles are in radians. because it's easier to calculate in head.
//		3.14159|Pi radians is half circle.
//		https://www.mathsisfun.com/numbers/pi.html
//		Get a plate and body measure then finding Pi yourself.
// 1a.	Measure around the edge (the circumference).
// 2a.	Measure across the circle (the diameter).
// 3a.	Divide circumference with diameter. (82 cm / 26 cm = 3.1538)
//
// 12.	Do the all matrix calculation for model(mesh transform),
//		view(camera), proj(squash it into 2D).
//		Note the vertices are mesh shape and we use the matrix4 to transform it.
// 13.	Bind shader program with glUseProgram().
// 14.	Send all of that into shader program and multiply it in the vertex shader.
//		gl_Position = proj * view * model * vec4(shape, 1.0f);
// 15.	Bind VAO with glBindVertexArray().
// 16.	Draw it out with "sizeof_indices" as the 2nd argument.
//		glDrawElements(GL_TRIANGLES, sizeof_indices, GL_UNSIGNED_INT, nullptr);
// 17.	Swap the canva with the one that you drew on.
//		glfwSwapBuffers(window);
// 18.	And clean out the hidden canva with the solid color.
//		glClearColor(bg_col, bg_col, bg_col, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
struct Mesh cgui_cook_vertices(struct Mesh* mesh)
{	cgui_unbindAll();
	glGenVertexArrays(1, &mesh->vao); // New VAO
	glBindVertexArray(mesh->vao); // Bind VAO

	glGenBuffers(1, &mesh->vbo); // New VBO
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo); // Bind VBO
	glBufferData(GL_ARRAY_BUFFER, mesh->sizeof_vertices, mesh->vertices, GL_STATIC_DRAW); // Add vertices
	// Set range of the vertices to read
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mesh->row * sizeof(GLfloat), NULL);
	mesh->index_attribute++;
	mesh->prefix_sum_attribute += 3;

	glGenBuffers(1, &mesh->ebo); // New EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo); // Bind EBO
	// Set range of the indices to read
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->sizeof_indices, mesh->indices, GL_STATIC_DRAW);

	return *mesh; }


struct Mesh cgui_add_attribute(GLint size, struct Mesh* mesh)
{	glEnableVertexAttribArray(mesh->index_attribute);
	glVertexAttribPointer(
	    mesh->index_attribute, size, GL_FLOAT, GL_FALSE,
	    mesh->row * sizeof(GLsizei),
	    (void*)(mesh->prefix_sum_attribute * sizeof(GLfloat))
	);
	mesh->index_attribute++;
	mesh->prefix_sum_attribute += size;
	return *mesh; }


char* cgui_read_file(char* path)
{	FILE* file;
	char* content = NULL;
	file = fopen(path, "r");

	if (NULL != file)
	{	fseek(file, 0, SEEK_END);
		int file_size = ftell(file);
		rewind(file);
		content = malloc(file_size + 1);

		if (NULL == content)
		{	printf("Memory allocation error.\n");
			free(content);
			fclose(file);
			// Do not crash the program.
			return ""; }

		size_t content_size = fread(content, 1, file_size, file);

		if (content_size != file_size)
		{	printf("Error reading the file.\n");
			free(content);
			fclose(file);
			return ""; }

		content[file_size] = '\0';

		fclose(file); }
	else
	{	printf("Not able to open the file '%s'.\n", path);
		return ""; }

	return content; }

GLuint cgui_init_shaders(char* vertex_path, char* fragment_path)
{	char* vertex_source = cgui_read_file(vertex_path);
	char* fragment_source = cgui_read_file(fragment_path);

	GLint has_compiled;
	GLchar info_log[1024];

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_source, NULL);
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &has_compiled);

	if (GL_FALSE == has_compiled)
	{	glGetShaderInfoLog(vertex_shader, 1024, NULL, info_log);
		printf("Vertex didn't compile!\n%s", info_log); }

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_source, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &has_compiled);

	if (GL_FALSE == has_compiled)
	{	glGetShaderInfoLog(fragment_shader, 1024, NULL, info_log);
		printf("Fragment didn't compile!\n%s", info_log); }

	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glGetProgramiv(shader_program, GL_LINK_STATUS, &has_compiled);

	if (GL_FALSE == has_compiled)
	{	glGetShaderInfoLog(shader_program, 1024, NULL, info_log);
		printf("Shader program didn't link!\n%s", info_log); }

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return shader_program; }


void cgui_set_uniform1f(char* name, float param0, GLuint shader_program)
{	GLuint uni1f = glGetUniformLocation(shader_program, name);
	glUniform1f(uni1f, param0); }


void cgui_set_uniform2f
(	char* name,
    float param0, float param1,
    GLuint shader_program)
{	GLuint uni2f = glGetUniformLocation(shader_program, name);
	glUniform2f(uni2f, param0, param1); }


void cgui_set_uniform3f
(	char* name,
    float param0, float param1, float param2,
    GLuint shader_program)
{	GLuint uni3f = glGetUniformLocation(shader_program, name);
	glUniform3f(uni3f, param0, param1, param2); }


void cgui_prtGLError()
{	GLenum err;

	while ((err = glGetError()) != GL_NO_ERROR) { printf("OpenGL Error: %d\n", err); } }
