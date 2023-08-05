#pragma once
#include <glad/glad.h>
#include <string>


std::string cgui_get_file_content(std::string file_name);
GLuint cgui_init_shaders(std::string vertex_file, std::string fragment_file);
void cgui_prtGLError();
void cgui_unbindAll();

#include "types.hpp"
void cgui_shader_update_view(GLuint shader_program, perspective *camera);
