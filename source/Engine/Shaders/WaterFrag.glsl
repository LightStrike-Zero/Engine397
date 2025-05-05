#version 410 core

in vec3 FragPos;
in vec3 Normal;

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

// Water properties
uniform vec3 waterColor = vec3(0.0, 0.4, 0.8);  // Base water color (blue)
uniform float waterAlpha = 0.6;                 // Water transparency
uniform float reflectivity = 0.6;               // How reflective the water is
uniform float shineDamper = 20.0;               // Controls the size of specular highlight

void main() {
    // Use the standard normal without distortion
    vec3 normal = normalize(Normal);

    // Directional light calculations
    vec3 lightDir = normalize(-light.direction);

    // Basic ambient light
    vec3 ambient = light.ambient * waterColor;

    // Diffuse lighting
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * waterColor;

    // Specular lighting (reflection)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shineDamper);

    // Simple Fresnel effect - more reflective at glancing angles
    float fresnelFactor = dot(viewDir, normal);
    fresnelFactor = clamp(pow(fresnelFactor, 1.0), 0.0, 1.0);
    vec3 specular = light.specular * spec * reflectivity * (1.0 - fresnelFactor);

    // Combine lighting components
    vec3 lighting = ambient + diffuse + specular;

    // Set final color with transparency
    FragColor = vec4(lighting, waterAlpha);
}