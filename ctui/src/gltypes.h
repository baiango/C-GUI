#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>


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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mesh->row * sizeof GLfloat, NULL);
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
	        mesh->row * sizeof GLsizei,
	        (void*)(mesh->prefix_sum_attribute * sizeof GLfloat)
	);
	mesh->index_attribute++;
	mesh->prefix_sum_attribute += size;
	return *mesh; }

//const char* read_file

//GLuint cgui_init_shaders(string vertex_file, string fragment_file) {
//
//}
