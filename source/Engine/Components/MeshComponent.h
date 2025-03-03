//
// Created by Shaun on 28/12/2024.
//

#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H
#include <vector>
#include "MeshData.h"


struct MeshComponent {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    unsigned int vao = 0;                                                           // Vertex Array Object ID
    unsigned int vbo = 0;                                                           // Vertex Buffer Object ID
    unsigned int ebo = 0;
    size_t indexCount = 0;

    // Constructor to initialize from RawMeshData
    MeshComponent(const RawMeshData& rawMeshData)
        : vertices(rawMeshData.vertices),
          indices(rawMeshData.indices),
          indexCount(rawMeshData.indices.size()) {}

};

#endif //MESHCOMPONENT_H
