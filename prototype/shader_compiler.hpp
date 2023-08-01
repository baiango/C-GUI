#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
using std::cout;
#include <fstream>
#include <string>
using std::string;

string get_file_content(string file_name);
GLuint *init_shaders(string vertex_file, string fragment_file);
void getGLError();
