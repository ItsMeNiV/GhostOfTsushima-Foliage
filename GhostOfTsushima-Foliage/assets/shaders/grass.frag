#version 430 core

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