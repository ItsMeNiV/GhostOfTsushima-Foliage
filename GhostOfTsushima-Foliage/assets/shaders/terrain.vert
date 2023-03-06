#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out VS_OUT {
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
} vs_out;

uniform mat4 viewProjection;
uniform mat4 model;
uniform sampler2D heightmapTexture;
uniform sampler2D normalmapTexture;

void main()
{
    float height = texture(heightmapTexture, aTexCoords).x * 10;

    vs_out.TexCoords = aTexCoords;
    vs_out.Normal = texture(normalmapTexture, aTexCoords).rgb;
    vs_out.Normal = normalize(vs_out.Normal * 2.0 - 1.0);
    vs_out.FragPos = vec3(model * vec4(aPos.x, height, aPos.y, 1.0));

    gl_Position = viewProjection * model * vec4(aPos.x, height, aPos.y, 1.0);
}