//
// Created by Shaun on 6/04/2025.
//

#ifndef DATATYPES_H
#define DATATYPES_H
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Texture;

struct MaterialData {
    glm::vec3 diffuseColor;         // Ambient color
    glm::vec3 specularColor;        // Specular color
    glm::vec3 ambientColor;         // Ambient color
    float shininess = 32.0f;        // Shininess value

    GLuint diffuseTextureID = 0; // OpenGL ID for the diffuseColor texture
    GLuint specularTextureID = 0; // OpenGL ID for the specularColor texture

    bool isDecal = false; // Decal flag


    MaterialData() = default;
    MaterialData(const glm::vec3& diffuse, const glm::vec3& specular, float shininess)
        : diffuseColor(diffuse), specularColor(specular), shininess(shininess) {}

};


struct RawMaterialData {
    GLenum baseColorTextureID = 0;  // Albedo texture
    GLenum metalnessTextureID = 0; // Metalness texture
    GLenum roughnessTextureID = 0; // Roughness texture
    GLenum normalTextureID = 0;    // Normal map
    bool isDecal = false;          // Decal flag
};


struct Vertex {
    glm::vec3 position;
    glm::vec2 texCoords;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct RawMeshData {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    RawMaterialData material;
    glm::mat4 transform = glm::mat4(1.0f); // Store the node's global transform

};


#endif //DATATYPES_H
