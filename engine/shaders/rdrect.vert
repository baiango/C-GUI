#version 330 core
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

layout (location = 0) in vec3 aPos;
uniform vec3 aColor;

out vec3 color;
out vec3 pos;

void main() {
	gl_Position = proj * view * model * vec4(aPos, 1.0f);
	color = aColor;
	pos = aPos;
}
