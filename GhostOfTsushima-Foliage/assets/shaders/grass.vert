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
out vec3 v_Normal;
out vec3 v_FragPos;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 renderTileModel;
uniform vec3 cameraPos;
uniform float time;

vec2 windDirection = normalize(vec2(1.5, 0.5));
float maxSway = 0.1;

vec3 toBezier(float yValue)
{
    vec3 pZero = vec3(0.0, 0.0, 0.0);
    vec3 pOne = vec3(0.0, 0.5, 0.0);
    vec3 pTwo = vec3(0.0, 1.0, maxSway);
    return (1-yValue) * ((1-yValue) * pZero + yValue*pOne) + yValue * ((1-yValue) * pOne + yValue*pTwo);
}
vec3 toBezierDerivative(float yValue)
{
    vec3 pZero = vec3(0.0, 0.0, 0.0);
    vec3 pOne = vec3(0.0, 0.5, 0.0);
    vec3 pTwo = vec3(0.0, 1.0, maxSway);
    return 2*(1-yValue) * (pOne-pZero) + 2*yValue*(pTwo-pOne);
}

void main()
{
    v_TexCoord = vec2(aTexCoord, 0.0);
    v_PixelHeight = aPos.y;

    // Swaying
    maxSway = abs(cos(((aInstanceHash / 1000000) + time*0.3)) / 10);

    vec3 position = aPos;
    position += toBezier(position.y);
    vec4 vertexWorldPos = renderTileModel * vec4(position + aInstancePositionoffset.xyz, 1.0);

    //Rotate towards camera
    vec3 cameraToVertex = normalize(vec3(vertexWorldPos) - cameraPos);
    vec3 up = normalize(toBezierDerivative(position.y));
    vec3 right = normalize(cross(cameraToVertex, up));
    cameraToVertex = normalize(cross(up, right));
    mat3 grassBladeModelMatrix = mat3(right, up, cameraToVertex);
    grassBladeModelMatrix[1][1] *= aInstanceHeight; //matrix [1][1] => Scale Y

    v_Normal = cameraToVertex;

    v_FragPos = vertexWorldPos.xyz;

    vec4 worldPos = renderTileModel * vec4((grassBladeModelMatrix * position) + aInstancePositionoffset.xyz, 1.0);
    vec4 screenSpacePos = projection * view * worldPos;

    gl_Position = screenSpacePos;
}