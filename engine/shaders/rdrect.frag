#version 330 core

in vec3 color;
in vec3 pos;

out vec4 FragColor;

float circle(in vec2 dist, in float radius){
	return 1.0 - smoothstep(radius - (radius * 0.01),
			radius + (radius * 0.01),
			dot(
				dist, dist
			) * 4.0);
}

float roundedFrame (vec2 pos, vec2 size, float radius, float thickness) {
	float d = length(max(abs(pos),size) - size) - radius;
	return smoothstep(0.55, 0.45, abs(d / thickness) * 5.0);
}

void main() {
	//float pct = circle(pos.xy, 0.5);
	//float pct = roundedFrame(pos.xy, vec2(0.45), 0.5, 0.5);
	float pct = log2(length(pos.xy) * 10.0) * -1;

	FragColor = vec4(color, 1.0f * pct);
}
