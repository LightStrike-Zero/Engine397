#version 410 core
#extension GL_ARB_separate_shader_objects : enable

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 LightSpacePos;
in vec3 Tangent;     // Texture coordinates
in vec3 BiTangent;     // Texture coordinates
in mat3 TBN;

layout(location = 0) out vec4 FragColor;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D roughnessMap;

struct Material {
    sampler2D diffuse;
    vec3 diffuseColor;
    vec3 specular;
    vec3 ambientColor;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float specularPower;
};
uniform Material material;
uniform DirectionalLight dirLight;
uniform vec3 viewPos;

// **New Uniform Added**
uniform sampler2DShadow shadowMap;// Shadow map texture
uniform vec2 gMapSize;

float shadowAmount = 0.7;

const float kPi = 3.14159265;
const float kShininess = 16.0;

struct Light {
    vec3 direction;
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

#define EPSILON 0.00001

float CalcShadowFactor(vec4 LightSpacePos)
{
    vec3 projCoords = LightSpacePos.xyz / LightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0 || projCoords.z < 0.0)
    return 0.0;

    vec3 lightDir = normalize(-light.direction);
    float bias = max(0.0025 * (1.0 - dot(Normal, lightDir)), 0.0005);

    float shadow = 0.0;
    int samples = 3;
    float texelSizeX = 1.0 / gMapSize.x;
    float texelSizeY = 1.0 / gMapSize.y;

    // Gaussian weights for smoother shadows
    float weights[3] = float[](0.25, 0.5, 0.25);

    for (int y = -1; y <= 1; y++)
    {
        for (int x = -1; x <= 1; x++)
        {
            vec2 offset = vec2(x * texelSizeX, y * texelSizeY);
            float weight = weights[x + 1] * weights[y + 1];
            // **Adjusted bias direction**
            shadow += texture(shadowMap, vec3(projCoords.xy + offset, projCoords.z - bias)) * weight;
        }
    }

    return shadow;
}

void main() {


    vec3 normal = texture(normalMap, TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0); // Convert normal from [0, 1] to [-1, 1]
    normal.y = -normal.y; // Flip Y-channel to fix inverted normals
    vec3 transformedNormal = normalize(TBN * normal);

    float roughness = texture(roughnessMap, TexCoords).r;


    vec3 lightDir;
    if (light.direction != vec3(0.0)) {
        // Directional light
        lightDir = normalize(-light.direction);
    } else {
        // Point light
        lightDir = normalize(light.position - FragPos);
    }

    // Transform light direction to tangent space
    vec3 tangentLightDir = normalize(TBN * lightDir);

    /* -------------------------
     * --Lighting Calculations--
     * -------------------------
     */

    // ambient lighting
    vec3 ambient = light.ambient * vec3(texture(albedoMap, TexCoords));

    // diffuse lighting
    float diff = max(dot(transformedNormal, tangentLightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(albedoMap, TexCoords));


    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 tangentViewDir = normalize(TBN * viewDir);


    const float kEnergyConservation = ( 8.0 + kShininess ) / ( 8.0 * kPi );
    vec3 halfwayDir = normalize(tangentLightDir + tangentViewDir);
    float spec = kEnergyConservation * pow(max(dot(transformedNormal, halfwayDir), 0.0), kShininess);

    vec3 specular = light.specular * spec;

    float shadow = CalcShadowFactor(LightSpacePos);
    vec3 lighting = ambient + shadow * (diffuse + specular);

//    FragColor = vec4(transformedNormal * 0.5 + 0.5, 1.0); // Visualize normals
//    FragColor = vec4(tangentLightDir * 0.5 + 0.5, 1.0);   // Visualize light direction

    // Shadow debug visualization
//    FragColor = vec4(vec3(CalcShadowFactor(LightSpacePos)), 1.0);

    // Lit view
    FragColor = vec4(lighting, 1.0);
    // No shadows
//    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
