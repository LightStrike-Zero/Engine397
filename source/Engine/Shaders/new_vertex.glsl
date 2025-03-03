#version 410 core

layout (location = 0) in vec3 aPos;       // Position
layout (location = 1) in vec3 aNormal;    // Normal
layout (location = 2) in vec2 aTexCoords; // Texture coordinates
layout (location = 3) in vec3 aTangent; // Texture coordinates
layout (location = 4) in vec3 aBiTangent; // Texture coordinates

out vec3 FragPos;       // Fragment position in world space
out vec3 Normal;        // Normal vector for lighting
out vec2 TexCoords;     // Texture coordinates
out vec4 LightSpacePos;    // Position transformed into light space
out vec3 Tangent;     // Texture coordinates
out vec3 BiTangent;     // Texture coordinates
out mat3 TBN;

uniform mat4 lightSpaceMatrix;  // Matrix to transform positions into light space
uniform mat4 model;      // Model transformation matrix
uniform mat4 view;       // View transformation matrix
uniform mat4 projection; // Projection matrix

void main()
{
    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 B = normalize(vec3(model * vec4(aBiTangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
    TBN = mat3(T, B, N);

    FragPos = vec3(model * vec4(aPos, 1.0)); // Transform vertex position to world space
    Normal = mat3(transpose(inverse(model))) * aNormal; // Correct normal for model transformations
    TexCoords = aTexCoords;

    // shadow mapping
    LightSpacePos = lightSpaceMatrix * vec4(FragPos, 1.0);

    gl_Position = projection * view * vec4(FragPos, 1.0); // Final vertex position
}
