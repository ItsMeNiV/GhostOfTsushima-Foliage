#version 430 core

#ifdef VERTEX

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
    float height = texture(heightmapTexture, aTexCoords).y * 15;

    vs_out.TexCoords = aTexCoords;
    vs_out.Normal = texture(normalmapTexture, aTexCoords).rgb;
    vs_out.Normal = normalize(vs_out.Normal * 2.0 - 1.0);
    vs_out.FragPos = vec3(model * vec4(aPos.x, height, aPos.y, 1.0));

    gl_Position = viewProjection * model * vec4(aPos.x, height, aPos.y, 1.0);
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

in VS_OUT {
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
} fs_in;

out vec4 FragColor;

uniform sampler2D diffuseTexture;
uniform vec3 viewPos;

DirectionalLight light;

void main()
{
    light.Direction = vec3(-0.1, -1.0, -0.7);
    light.Ambient = vec3(0.21, 0.11, 0.04);
    light.Diffuse = vec3(0.36, 0.29, 0.23);
    //light.Specular = vec3(1, 0.96, 0.93);

	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 normal = normalize(fs_in.Normal);

	vec3 lightDir = normalize(-light.Direction);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(normal, lightDir), 0.0);
    //float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);

    vec3 terrainColor = texture(diffuseTexture, fs_in.TexCoords).rgb;

    vec3 ambient = light.Ambient;
    vec3 diffuse = light.Diffuse * diff;
    //vec3 specular = light.Specular * spec;

    vec3 fragResult = (ambient + diffuse) * (terrainColor*0.5);
	
	FragColor = vec4(fragResult, 1.0);
}

#endif