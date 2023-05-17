#version 430 core
out vec4 FragColor;

in vec2 vTexCoords;

uniform sampler2D screenTexture;

void main() {
	const float gamma = 2.2;
	const float exposure = 1.0;
	vec3 hdrColor = texture(screenTexture, vTexCoords).rgb;

	vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
	mapped = pow(mapped, vec3(1.0/gamma));

	FragColor = vec4(mapped, 1.0);
}