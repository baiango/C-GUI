#version 330 core

in vec3 color;
in vec3 pos;
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

void main() {
	// I clamped the roundness because it works in this range only.
	float rd_clamped = clamp(
		roundness,
		0.0f,
		min(canva_dimension.x, canva_dimension.y) / 2.0f
	);
	vec2 canva_radius = canva_dimension / 2.0f;

	float left = pos.x + canva_radius.x - rd_clamped;
	float right = pos.x - canva_radius.x + rd_clamped;
	float top = pos.y - canva_radius.y + rd_clamped;
	float bottom = pos.y + canva_radius.y - rd_clamped;

	float canva;
	canva += circle(left, top, rd_clamped);
	canva += circle(left, bottom, rd_clamped);
	canva += circle(right, top, rd_clamped);
	canva += circle(right, bottom, rd_clamped);
	canva += compare(pos.x, 0.0f, canva_radius.x - rd_clamped);
	canva += compare(pos.y, 0.0f, canva_radius.y - rd_clamped);

	FragColor = vec4(color, 1.0f * canva);
}
