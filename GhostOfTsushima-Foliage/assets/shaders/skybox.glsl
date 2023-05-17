#version 430 core

#ifdef VERTEX

layout (location = 0) in vec3 aPos;

out vec3 vTexCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
	vTexCoords = aPos;
	vec4 pos = projection * view * vec4(aPos, 1.0);
	gl_Position = pos.xyww;
}

#endif

//=========================//

#ifdef FRAGMENT

out vec4 FragColor;

in vec3 vTexCoords;

uniform samplerCube skybox;

void main() {
	FragColor = texture(skybox, vTexCoords);
}

#endif