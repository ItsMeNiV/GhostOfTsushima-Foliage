#version 430 core

#ifdef VERTEX

layout (location = 0) in vec3 aPos;

uniform mat4 viewProjection;

void main() {
	gl_Position = viewProjection * vec4(aPos, 1.0);
}

#endif

//=========================//

#ifdef FRAGMENT

out vec4 FragColor;

uniform vec3 color;

void main() {
	FragColor = vec4(color, 1.0);
}

#endif