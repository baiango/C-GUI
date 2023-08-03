#version 330 core
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 color;

uniform float scale;


void main() {
	gl_Position = proj * view * model * vec4(aPos, 1.0f);
	color = aColor;
}
