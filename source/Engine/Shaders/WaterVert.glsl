#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec3 WorldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time; 

void main()
{
    // Start with the original position
    vec3 position = aPos;

    // Add subtle wave animation
    float waveHeight = 0.5;  // Height of waves (small for subtle effect)
    float waveFrequency = 0.2;  // Wave frequency
    float waveSpeed = 1.5;     // Speed of waves

    // Calculate wave pattern - combine multiple sine waves for more natural look
    float wave1 = sin(position.x * waveFrequency + time * waveSpeed) *
    cos(position.z * waveFrequency * 0.8 + time * waveSpeed * 0.8);
    float wave2 = sin(position.z * waveFrequency * 1.2 + time * waveSpeed * 1.3) * 0.5;

    // Apply wave to Y position
    position.y += waveHeight * (wave1 + wave2);

    // Calculate normal perturbation
    vec3 modifiedNormal = aNormal;
    // Compute wave normals (approximate derivative of the height function)
    if (abs(aNormal.y) > 0.9) {  // Only adjust normals that point mostly up
        float normalStrength = 0.8;
        modifiedNormal.x -= normalStrength * waveHeight * cos(position.x * waveFrequency + time * waveSpeed) *
        cos(position.z * waveFrequency * 0.8 + time * waveSpeed * 0.8) * waveFrequency;
        modifiedNormal.z -= normalStrength * waveHeight * sin(position.z * waveFrequency * 1.2 + time * waveSpeed * 1.3) *
        waveFrequency * 1.2 * 0.5;
        modifiedNormal = normalize(modifiedNormal);
    }

    // Transform to world space
    WorldPos = vec3(model * vec4(position, 1.0));
    FragPos = WorldPos;
    Normal = mat3(transpose(inverse(model))) * modifiedNormal;

    // Final position
    gl_Position = projection * view * vec4(FragPos, 1.0);
}