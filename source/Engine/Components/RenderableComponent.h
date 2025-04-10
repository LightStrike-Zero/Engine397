//
// Created by Shaun on 28/12/2024.
//

/**
 * @file RenderableComponent.h
 * @brief Defines the Renderable Component class used to mesh data that can be rendered in the scene.
 * @author Shaun
 * @date 2024-12-28
 */

#ifndef RENDERABLECOMPONENT_H
#define RENDERABLECOMPONENT_H
#include <memory>
#include <vector>

#include "DataTypes.h"
#include "Interfaces/IMeshBuffer.h"

/**
 * @class RenderableComponent
 * @brief Represents mesh data that can be rendered in the scene.
 *
 * This component holds the raw vertex and index data for a mesh,
 * along with a pointer to the mesh buffer used by the renderer.
 */
class RenderableComponent
{
public:
    /** @brief Vertex data for the mesh */
    std::vector<Vertex> vertices;

    /** @brief Index data for the mesh */
    std::vector<unsigned int> indices;

    /** @brief GPU buffer that stores vertex/index data for rendering */
    std::shared_ptr<IMeshBuffer> meshBuffer;

    /**
     * @brief Constructor that initialises the component using raw mesh data.
     * @param rawMeshData The input vertex/index data used to populate the component.
     */
    explicit RenderableComponent(const RawMeshData& rawMeshData)
        : vertices(rawMeshData.vertices),
          indices(rawMeshData.indices)
    {
    }

};

#endif //RENDERABLECOMPONENT_H
