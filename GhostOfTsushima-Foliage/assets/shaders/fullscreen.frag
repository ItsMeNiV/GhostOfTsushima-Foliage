#version 430 core
out vec4 FragColor;

in vec2 vTexCoords;

uniform sampler2D screenTexture;

void main() {
	FragColor = texture(screenTexture, vTexCoords);
}