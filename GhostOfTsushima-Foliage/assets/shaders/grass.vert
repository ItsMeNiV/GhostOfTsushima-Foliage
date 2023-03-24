#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in float aTexCoord;
layout (location = 2) in float aSwayStrength;
// Instancedata
layout (location = 3) in vec4 aInstancePositionoffset;
layout (location = 4) in float aInstanceHeight;
layout (location = 5) in uint aInstanceHash;

out vec2 v_TexCoord;
out float v_PixelHeight;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 renderTileModel;
uniform vec3 cameraPos;
uniform float time;

vec2 windDirection = normalize(vec2(1.5, 0.5));

void main()
{
    v_TexCoord = vec2(aTexCoord, 0.0);
    v_PixelHeight = aPos.y;
    
    vec4 vertexWorldPos = renderTileModel * vec4(aPos + aInstancePositionoffset.xyz, 1.0);

    //Rotate towards camera
    vec3 cameraToVertex = normalize(vec3(vertexWorldPos) - cameraPos);
    vec3 up = vec3(0,1,0);
    vec3 right = normalize(cross(cameraToVertex, up));
    cameraToVertex = normalize(cross(up, right));
    mat3 grassBladeModelMatrix = mat3(right, up, cameraToVertex);
    grassBladeModelMatrix[1][1] *= aInstanceHeight; //matrix [1][1] => Scale Y

    // Swaying
    float trigValue = cos(((aInstanceHash / 10000) + time) * 0.5);
    trigValue = (trigValue * trigValue * 0.65);
    vec3 position = aPos;
    position.xz += aSwayStrength * trigValue * 0.6 * windDirection;

    vec4 screenSpacePos = projection * view * renderTileModel * vec4((grassBladeModelMatrix * position) + aInstancePositionoffset.xyz, 1.0);

    gl_Position = screenSpacePos;
}