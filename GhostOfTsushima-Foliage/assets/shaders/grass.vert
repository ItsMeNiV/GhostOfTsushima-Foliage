#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in float aTexCoord;
layout (location = 2) in float aSwayStrength;

out vec2 v_TexCoord;

uniform mat4 viewProjection;
uniform mat4 model;
uniform float time;

void main()
{
    v_TexCoord = vec2(aTexCoord, 0.0);
    float trigValue = cos(time * 0.5);
    trigValue = (trigValue * trigValue * 0.65) * 0.5;
    vec3 position = aPos;
    position.xz += aSwayStrength * trigValue * 0.6;
    gl_Position = viewProjection * model * vec4(position, 1.0);
}