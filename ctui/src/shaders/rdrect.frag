#version 330 core

in vec3 color;
in vec3 shape;
in vec2 canva_dimension;
in float roundness; // 0.0f..0.5f

out vec4 FragColor;


float circle(in float pos_x, in float pos_y, in float radius) {
	float ret = length(vec2(pos_x, pos_y));
	return (radius > ret) ? 1.0f : 0.0f;
}

float compare(in float a, in float b, in float epsilon) {
	return (a > b - epsilon && a < b + epsilon) ? 1.0f : 0.0f;
}

float map_range(in float value, in float a_min, in float a_max, in float b_min, in float b_max) {
	return b_min + (b_max - b_min) * ((value - a_min) / (a_max - a_min));
}

void main() {
	float roundness_max = min(canva_dimension.x, canva_dimension.y) / 2.0f;

	float rd_clamped = clamp(
		map_range(roundness, 0.0f, 0.5f, 0.0f, roundness_max),
		0.0f,
		roundness_max
	);
	vec2 canva_radius = canva_dimension / 2.0f;

	float left = shape.x + canva_radius.x - rd_clamped;
	float right = shape.x - canva_radius.x + rd_clamped;
	float top = shape.y - canva_radius.y + rd_clamped;
	float bottom = shape.y + canva_radius.y - rd_clamped;

	float canva;
	canva += circle(left, top, rd_clamped);
	canva += circle(left, bottom, rd_clamped);
	canva += circle(right, top, rd_clamped);
	canva += circle(right, bottom, rd_clamped);
	canva += compare(shape.x, 0.0f, canva_radius.x - rd_clamped);
	canva += compare(shape.y, 0.0f, canva_radius.y - rd_clamped);

	FragColor = vec4(color, 1.0f * canva);
}
