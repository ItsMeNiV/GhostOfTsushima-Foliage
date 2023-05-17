#version 430 core

#ifdef VERTEX

layout (location = 0) in vec3 aPos;
layout (location = 1) in float aTexCoord;
layout (location = 2) in float aSwayStrength;
// Instancedata
layout (location = 3) in vec4 aInstancePositionoffset;
layout (location = 4) in float aInstanceHeight;
layout (location = 5) in uint aInstanceHash;

out vec2 v_TexCoord;
out float v_PixelHeight;
out mat3 v_TBN;
out vec3 v_FragPos;
out vec3 v_Test;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 renderTileModel;
uniform vec3 cameraPos;
uniform float time;

vec2 windDirection = normalize(vec2(1.5, 1.5));
float maxSway = 1.0;

vec3 toBezier(float yValue)
{
    vec3 pZero = vec3(0.0, 0.0, 0.0);
    vec3 pOne = vec3(0.0, 0.5, 0.0);
    vec3 pTwo = vec3(maxSway * windDirection.x, 1.0, maxSway * windDirection.y);
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

    //TEST
    windDirection = normalize(vec2(sin(time), cos(time)));

    // Swaying
    maxSway = abs(cos((((aInstanceHash>>10000) + time) * 0.3)) / 10);

    vec3 position = aPos;
    position += toBezier(position.y);
    vec4 vertexWorldPos = renderTileModel * vec4(position + aInstancePositionoffset.xyz, 1.0);

    //Rotate towards camera
    vec3 cameraToVertex = normalize(vec3(vertexWorldPos) - cameraPos);
    vec3 up = normalize(toBezierDerivative(position.y));
    //vec3 right = normalize(cross(cameraToVertex, up));
    vec3 right = vec3(view[0][0], view[1][0], view[2][0]);
    //cameraToVertex = normalize(cross(up, right));
    mat3 grassBladeModelMatrix = mat3(right, up, cameraToVertex);
    grassBladeModelMatrix[1][1] *= aInstanceHeight; //matrix [1][1] => Scale Y

    vec3 normal = normalize(cross(up, right));
    vec3 tangent = right;
    vec3 bitangent = normalize(cross(normal, tangent));
    vec3 T = normalize(grassBladeModelMatrix * tangent);
    vec3 B = normalize(grassBladeModelMatrix * bitangent);
    vec3 N = normalize(grassBladeModelMatrix * normal);
    v_TBN = mat3(T, B, N);

    v_FragPos = vertexWorldPos.xyz;

    vec4 worldPos = renderTileModel * vec4((grassBladeModelMatrix * position) + aInstancePositionoffset.xyz, 1.0);
    vec4 screenSpacePos = projection * view * worldPos;

    gl_Position = screenSpacePos;
}

#endif

//=========================//

#ifdef FRAGMENT

struct DirectionalLight
{
	vec3 Direction;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

in vec2 v_TexCoord;
in float v_PixelHeight;
in mat3 v_TBN;
in vec3 v_FragPos;
in vec3 v_Test;

out vec4 FragColor;

uniform sampler2D grassbladeTexture;
uniform sampler2D grassbladeNormalMap;
uniform vec3 cameraPos;

DirectionalLight light;

void main()
{
    light.Direction = vec3(-0.1, -1.0, -0.5);
    light.Ambient = vec3(0.61, 0.32, 0.1);
    light.Diffuse = vec3(1, 0.86, 0.73);
    light.Specular = vec3(1, 0.96, 0.93);

	vec3 viewDir = normalize(cameraPos - v_FragPos);
    vec3 normal = texture(grassbladeNormalMap, v_TexCoord).rgb;
    normal = normal * 2.0 - 1.0;   
    normal = normalize(v_TBN * normal);

	vec3 lightDir = normalize(-light.Direction);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 128);

	vec3 grassColor = mix(vec3(0.0), texture(grassbladeTexture, v_TexCoord).rgb, v_PixelHeight+0.1);

    vec3 ambient = light.Ambient;
    vec3 diffuse = light.Diffuse * diff;
    vec3 specular = light.Specular * spec;

    vec3 fragResult = (ambient + diffuse + specular) * grassColor;

	
	//FragColor = vec4(v_Test, 1.0);
	FragColor = vec4(fragResult, 1.0);
}

#endif