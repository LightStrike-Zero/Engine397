//
// Created by Shaun on 28/12/2024.
//

#ifndef RENDERABLECOMPONENT_H
#define RENDERABLECOMPONENT_H
#include <memory>
#include <vector>

#include "DataTypes.h"
#include "Interfaces/IMeshBuffer.h"


class RenderableComponent
{
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::shared_ptr<IMeshBuffer> meshBuffer;

    // Constructor to initialize from RawMeshData
    explicit RenderableComponent(const RawMeshData& rawMeshData)
        : vertices(rawMeshData.vertices),
          indices(rawMeshData.indices)
    {
    }

};

#endif //RENDERABLECOMPONENT_H
