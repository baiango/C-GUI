#version 330 core
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

layout (location = 0) in vec3 aShape;
uniform vec3 aColor;
uniform vec2 aCanvaDimension;
uniform float aRoundness;

out vec3 color;
out vec3 shape;
out vec2 canva_dimension;
out float roundness;


void main() {
	gl_Position = proj * view * model * vec4(aShape, 1.0f);
	shape = aShape;
	color = aColor;
	canva_dimension = aCanvaDimension;
	roundness = aRoundness;
}
