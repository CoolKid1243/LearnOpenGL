#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[1];
uniform Material material;
uniform bool gammaEnabled;

// Function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // Sample and optionally linearize
    vec3 color = texture(material.diffuse, TexCoords).rgb;
    if (gammaEnabled) {
        color = pow(color, vec3(2.2));
    }

    // Lighting calculations (in linear space)
    vec3 norm    = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result  = CalcDirLight(dirLight, norm, viewDir);
    for (int i = 0; i < 1; ++i) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    // Gamma correction back to sRGB
    if (gammaEnabled) {
        result = pow(result, vec3(1.0/2.2));
    }

    FragColor = vec4(result, 1.0);
}

// Calculates directional lighting
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff    = max(dot(normal, lightDir), 0.0);
    vec3 halfway  = normalize(lightDir + viewDir);
    float spec    = pow(max(dot(normal, halfway), 0.0), material.shininess);

    vec3 ambient  = light.ambient  * texture(material.diffuse, TexCoords).rgb;
    vec3 diffuse  = light.diffuse  * diff * texture(material.diffuse, TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    return ambient + diffuse + specular;
}

// Calculates point lighting
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff    = max(dot(normal, lightDir), 0.0);
    vec3 halfway  = normalize(lightDir + viewDir);
    float spec    = pow(max(dot(normal, halfway), 0.0), material.shininess);

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient  = light.ambient  * texture(material.diffuse, TexCoords).rgb;
    vec3 diffuse  = light.diffuse  * diff * texture(material.diffuse, TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}
