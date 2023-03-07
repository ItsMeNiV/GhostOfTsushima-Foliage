#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in float aTexCoord;

out vec2 v_TexCoord;

uniform mat4 viewProjection;
uniform mat4 model;

void main()
{
    v_TexCoord = vec2(aTexCoord, 0.0);
    gl_Position = viewProjection * model * vec4(aPos, 1.0);
}