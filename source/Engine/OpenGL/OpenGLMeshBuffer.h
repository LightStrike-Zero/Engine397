//
// Created by Shaun on 7/03/2025.
//

/**
 * @file OpenGLMeshBuffer.h
 * @brief Defines the OpenGL implementation, inherits from IMeshBuffer for managing mesh data on the GPU.
 * @author Shaun
 * @date 2025-03-07
 */

#ifndef OPENGLMESHBUFFER_H
#define OPENGLMESHBUFFER_H
#include "Components/RenderableComponent.h"
#include "Interfaces/IMeshBuffer.h"

/**
 * @class OpenGLMeshBuffer
 * @brief Represents GPU-side mesh buffer: OpenGL Vertex Array, Vertex Buffer, and Element Buffer.
 */
class OpenGLMeshBuffer : public IMeshBuffer
{
public:
    /**
     * @brief Constructs an OpenGL mesh buffer.
     * @param numIndices Number of indices in the index buffer.
     * @param numVertices Number of vertices in the vertex buffer.
     * @param indexData Pointer to the array of indices.
     * @param vertexData Pointer to the array of vertex data.
     */
    OpenGLMeshBuffer(int numIndices, int numVertices, std::vector<unsigned int>::pointer indexData, std::vector<Vertex>::pointer vertexData);

    /**
     * @brief Binds the VAO for rendering.
     */
    void bind() override;

    /**
     * @brief Draws the mesh using the bound VAO.
     */
    void draw() override;

    /**
     * @brief Destructor that cleans up GPU resources.
     */
    ~OpenGLMeshBuffer();

    /**
     * @brief Retrieves the VAO ID.
     * @return The OpenGL VAO ID.
     */
    unsigned int getVAO() const { return m_vao; }

// private: //TODO Does this stay private?
    unsigned int m_vao, m_vbo, m_ebo;
    bool m_lineMode;
    size_t m_indexCount;
};

#endif //OPENGLMESHBUFFER_H
