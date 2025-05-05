#version 410 core

layout (location = 0) in vec3 aPos;       // Position
layout (location = 1) in vec3 aNormal;    // Normal

out vec3 FragPos;       // Fragment position in world space
out vec3 Normal;        // Normal vector for lighting

uniform mat4 model;      // Model transformation matrix
uniform mat4 view;       // View transformation matrix
uniform mat4 projection; // Projection matrix

void main()
{
    // No wave animation - just use the original position
    vec3 position = aPos;

    FragPos = vec3(model * vec4(position, 1.0)); // Transform vertex position to world space
    Normal = mat3(transpose(inverse(model))) * aNormal; // Correct normal for model transformations

    gl_Position = projection * view * vec4(FragPos, 1.0); // Final vertex position
}