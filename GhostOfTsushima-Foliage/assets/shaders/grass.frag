#version 330 core

in vec2 v_TexCoord;

out vec4 FragColor;

uniform sampler2D grassbladeTexture;

void main()
{
	FragColor = texture(grassbladeTexture, v_TexCoord);
}