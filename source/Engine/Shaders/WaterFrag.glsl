#version 410 core

in vec3 FragPos;
in vec3 Normal;
in vec4 ClipSpace;
in vec3 WorldPos;

layout(location = 0) out vec4 FragColor;

struct Light {
    vec3 direction;
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform vec3 viewPos;
uniform float time;

// Water properties
uniform vec3 waterColor;
uniform float waterAlpha;
uniform float reflectivity;
uniform float shineDamper;

void main() {
    vec3 normal = normalize(Normal);

    // Add subtle normal variation for water effect
    float normalStrength = 0.1;
    float t = time * 0.5;
    normal.xz += normalStrength * vec2(
    sin(WorldPos.x * 2.0 + t) * cos(WorldPos.z * 2.0 + t),
    cos(WorldPos.x * 2.0 + t) * sin(WorldPos.z * 2.0 + t)
    );
    normal = normalize(normal);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lightDir = normalize(-light.direction);

    // Basic lighting
    vec3 ambient = light.ambient * waterColor;

    float diff = max(dot(normal, lightDir), 0.0) * 0.5;
    vec3 diffuse = light.diffuse * diff * waterColor;

    // Specular highlights
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shineDamper);
    vec3 specular = light.specular * spec;

    // Fresnel effect - more reflective at glancing angles
    float fresnel = pow(1.0 - max(dot(normal, viewDir), 0.0), 2.0) * 0.5;

    vec3 result = ambient + diffuse + specular;

    // Apply fresnel to strengthen reflections at glancing angles
    result = mix(result, vec3(1.0), fresnel * reflectivity * 0.2);

    // Final color with alpha
    FragColor = vec4(result, waterAlpha);
}