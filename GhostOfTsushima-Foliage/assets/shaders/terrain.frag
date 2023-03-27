#version 430 core

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
    light.Ambient = vec3(0.04, 0.02, 0.01);
    light.Diffuse = vec3(1.0, 0.81, 0.64);
    light.Specular = vec3(1.0, 0.81, 0.64);

	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 normal = normalize(fs_in.Normal);

	vec3 lightDir = normalize(-light.Direction);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);

    vec3 terrainColor = texture(diffuseTexture, fs_in.TexCoords).rgb;

    vec3 ambient = light.Ambient;
    vec3 diffuse = light.Diffuse * diff;
    vec3 specular = light.Specular * spec;

    vec3 fragResult = (ambient + diffuse + specular) * terrainColor;
	
	FragColor = vec4(fragResult, 1.0);
}