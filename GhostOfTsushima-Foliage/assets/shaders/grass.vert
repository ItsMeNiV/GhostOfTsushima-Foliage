#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in float aTexCoord;
layout (location = 2) in float aSwayStrength;
// Instancedata
layout (location = 3) in vec3 aInstancePositionoffset;
layout (location = 4) in uint aInstanceHash;

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
    
    vec4 vertexPos = renderTileModel * vec4(aPos + aInstancePositionoffset, 1.0);

    vec3 fwd = normalize(vec3(vertexPos) - cameraPos);
    vec3 up = vec3(0,1,0);
    vec3 right = normalize(cross(fwd, up));
    fwd = normalize(cross(up, right)); // updating fwd to make sure its perpendicular to both up and right
    mat3 rotationMatrix = mat3(right, up, fwd);

    float trigValue = cos(((aInstanceHash / 10000) + time) * 0.5);
    trigValue = (trigValue * trigValue * 0.65);
    vec3 position = aPos;
    position.xz += aSwayStrength * trigValue * 0.6 * windDirection;

    vec4 worldSpacePos = projection * view * renderTileModel * vec4((rotationMatrix * position) + aInstancePositionoffset, 1.0);

    gl_Position = worldSpacePos;
}