//
// Created by Shaun on 7/03/2025.
//

#ifndef OPENGLMESHBUFFER_H
#define OPENGLMESHBUFFER_H
#include "Components/RenderableComponent.h"
#include "Interfaces/IMeshBuffer.h"

class OpenGLMeshBuffer : public IMeshBuffer
{
public:
    OpenGLMeshBuffer(int numIndices, int numVertices, std::vector<unsigned int>::pointer indexData, std::vector<Vertex>::pointer vertexData);

    void bind() override;

    // move this later
    void draw() override;

    ~OpenGLMeshBuffer();

    unsigned int getVAO() const { return m_vao; }

// private:
    unsigned int m_vao, m_vbo, m_ebo;
    size_t m_indexCount;
};

#endif //OPENGLMESHBUFFER_H
