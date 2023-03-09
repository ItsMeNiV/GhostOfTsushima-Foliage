#version 330 core

in vec2 v_TexCoord;
in float v_PixelHeight;

out vec4 FragColor;

uniform sampler2D grassbladeTexture;

void main()
{
	vec3 finalColor = mix(vec3(0.0), texture(grassbladeTexture, v_TexCoord).rgb, v_PixelHeight);
	finalColor = mix(vec3(1.0), texture(grassbladeTexture, v_TexCoord).rgb, -v_PixelHeight+1.7);
	FragColor = vec4(finalColor, 1.0);
}