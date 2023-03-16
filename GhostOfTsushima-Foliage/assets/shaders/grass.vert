#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in float aTexCoord;
layout (location = 2) in float aSwayStrength;
layout (location = 3) in mat4 aInstanceMatrix;

out vec2 v_TexCoord;
out float v_PixelHeight;

uniform mat4 viewProjection;
uniform float time;

uniform sampler2D terrainHeightmapTexture;

void main()
{
    v_TexCoord = vec2(aTexCoord, 0.0);
    v_PixelHeight = aPos.y;
    float trigValue = cos(time * 0.5 + gl_InstanceID);
    trigValue = (trigValue * trigValue * 0.65);
    vec3 position = aPos;
    position.xz += aSwayStrength * trigValue * 0.6;
    gl_Position = viewProjection * aInstanceMatrix * vec4(position, 1.0);
}