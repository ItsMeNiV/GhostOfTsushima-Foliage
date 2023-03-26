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
in vec3 v_Normal;
in vec3 v_FragPos;

out vec4 FragColor;

uniform sampler2D grassbladeTexture;
uniform vec3 cameraPos;

DirectionalLight light;

void main()
{
light.Direction = vec3(-0.1, -1.0, -0.7);
    light.Ambient = vec3(0.91, 0.47, 0.16);
    light.Diffuse = vec3(1.0, 0.81, 0.64);
    light.Specular = vec3(1.0, 0.81, 0.64);

	vec3 viewDir = normalize(cameraPos - v_FragPos);
    vec3 normal = normalize(v_Normal);

	vec3 lightDir = normalize(-light.Direction);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);

    vec3 grassColor = mix(vec3(0.0), texture(grassbladeTexture, v_TexCoord).rgb, v_PixelHeight);
	grassColor = mix(vec3(1.0), texture(grassbladeTexture, v_TexCoord).rgb, -v_PixelHeight+1.7);

    vec3 ambient = light.Ambient * grassColor;
    vec3 diffuse = light.Diffuse * diff * grassColor;
    vec3 specular = light.Specular * spec;

    vec3 fragResult = (ambient + diffuse + specular) * grassColor;

	
	FragColor = vec4(fragResult, 1.0);
}