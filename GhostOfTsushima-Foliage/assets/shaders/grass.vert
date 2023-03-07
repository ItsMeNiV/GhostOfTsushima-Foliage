#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in float aTexCoord;
layout (location = 2) in float aSwayStrength;

out vec2 v_TexCoord;

uniform mat4 viewProjection;
uniform mat4 model;
uniform float time;

uniform sampler2D terrainHeightmapTexture;
uniform vec2 offsets[1000];

void main()
{
    vec2 offset = offsets[gl_InstanceID];
    v_TexCoord = vec2(aTexCoord, 0.0);
    float trigValue = cos(time * 0.5 + gl_InstanceID);
    trigValue = (trigValue * trigValue * 0.65);
    vec3 position = aPos;
    position.xz += aSwayStrength * trigValue * 0.6;
    position.xz += offset;
    gl_Position = viewProjection * model * vec4(position, 1.0);
}