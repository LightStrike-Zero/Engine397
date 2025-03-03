//
// Created by Shaun on 29/11/2024.
//

#ifndef MESHDATA_H
#define MESHDATA_H
#include <string>

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

#endif //MESHDATA_H
