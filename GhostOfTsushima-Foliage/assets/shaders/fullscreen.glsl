#version 430 core

#ifdef VERTEX

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 vTexCoords;

void main() {
	vTexCoords = aTexCoords;
	gl_Position = vec4(aPos, 0.0, 1.0);
}

#endif

//=========================//

#ifdef FRAGMENT

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

#endif