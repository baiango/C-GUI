#pragma once
#include <glad/glad.h>
#include <string>


std::string get_file_content(std::string file_name);
GLuint init_shaders(std::string vertex_file, std::string fragment_file);
void prtGLError();
void unbindAll();

#include "types.hpp"
void update_view(GLuint shader_program, perspective *camera);
