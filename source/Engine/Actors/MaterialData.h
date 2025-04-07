// //
// // Created by Shaun on 29/11/2024.
// //
//
// #ifndef MATERIALDATA_H
// #define MATERIALDATA_H
//
// #include <glm/glm.hpp>
// #include <string>
// #include <GL/glew.h>
//
// class Texture;
//
// struct MaterialData {
//     glm::vec3 diffuseColor; // Ambient color
//     glm::vec3 specularColor;        // Specular color
//     glm::vec3 ambientColor;         // Ambient color
//     float shininess = 32.0f;                // Shininess value
//
//     GLuint diffuseTextureID = 0; // OpenGL ID for the diffuseColor texture
//     GLuint specularTextureID = 0; // OpenGL ID for the specularColor texture
//
//     bool isDecal = false; // Decal flag
//
//
//     MaterialData() = default;
//     MaterialData(const glm::vec3& diffuse, const glm::vec3& specular, float shininess)
//         : diffuseColor(diffuse), specularColor(specular), shininess(shininess) {}
//
// };
//
//
// struct RawMaterialData {
//     GLuint baseColorTextureID = 0;  // Albedo texture
//     GLuint metalnessTextureID = 0; // Metalness texture
//     GLuint roughnessTextureID = 0; // Roughness texture
//     GLuint normalTextureID = 0;    // Normal map
//     bool isDecal = false;          // Decal flag
// };
//
//
// #endif //MATERIALDATA_H
