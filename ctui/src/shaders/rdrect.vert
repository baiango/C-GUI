#version 330 core
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

layout (location = 0) in vec3 aPos;
uniform vec3 aColor;
uniform vec2 aCanvaDimension;
uniform float aRoundness;

out vec3 color;
out vec3 pos;
out vec2 canva_dimension;
out float roundness;


void main() {
	gl_Position = proj * view * model * vec4(aPos, 1.0f);
	pos = aPos;
	color = aColor;
	canva_dimension = aCanvaDimension;
	roundness = aRoundness;
}
