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


struct mat4
{	float x0, y0, z0, w0;
	float x1, y1, z1, w1;
	float x2, y2, z2, w2;
	float x3, y3, z3, w3; };


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
	{	printf("Not able to open the file '%s'.\n", path); }

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
		printf("Shader program didn't link!\n", info_log); }

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
