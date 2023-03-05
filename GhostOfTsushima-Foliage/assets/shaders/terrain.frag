#version 330 core

in vec2 v_TexCoords;

out vec4 FragColor;

uniform vec3 color;
uniform sampler2D diffuseTexture;

void main()
{
	
	FragColor = vec4(texture(diffuseTexture, v_TexCoords));
}