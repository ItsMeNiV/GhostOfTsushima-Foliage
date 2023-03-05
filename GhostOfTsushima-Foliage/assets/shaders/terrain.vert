#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 viewProjection;
uniform mat4 model;
uniform sampler2D heightmapTexture;

void main()
{
    float height = texture(heightmapTexture, aTexCoords).x * 10;
    gl_Position = viewProjection * model * vec4(aPos.x, height, aPos.y, 1.0);
}