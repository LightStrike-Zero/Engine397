
#include "OpenGLMeshBuffer.h"
#include <GL/glew.h> 

#include "Systems/EventSystem.h"

OpenGLMeshBuffer::OpenGLMeshBuffer(const int numIndices, const int numVertices, const std::vector<unsigned int>::pointer indexData, const std::vector<Vertex>::pointer vertexData)
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    // Upload vertex data to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), vertexData, GL_STATIC_DRAW);

    // Upload index data to the GPU
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indexData, GL_STATIC_DRAW);

    // Set vertex attribute pointers
    /*
     * Vertex array will look something like this
     * --------------------------------------------------------
     * | Position | Normal | TexCoords | Tangent | Bi-tangent |
     * --------------------------------------------------------
     * |   0      |   1    |     2     |    3    |     4      |
     * --------------------------------------------------------
     */
    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1); // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2); // TexCoords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glEnableVertexAttribArray(3); // Tangent
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    glEnableVertexAttribArray(4); // Bi-tangent
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    // Unbind VAO
    glBindVertexArray(0);

    // Set the index count
    m_indexCount = numIndices;

    m_lineMode = false;
        
    // Register for draw mode change events
    EventSystem::getInstance().addListener(
        EventType::DrawModeChanged,
        [this](const Event& event) {
            auto& drawEvent = static_cast<const DrawModeChangedEvent&>(event);
            m_lineMode = drawEvent.lineMode;
        }
    );
}

void OpenGLMeshBuffer::bind()
{
    glBindVertexArray(m_vao);
}

void OpenGLMeshBuffer::draw()
{
    if (m_lineMode) {
        glDrawElements(GL_LINES, static_cast<GLsizei>(m_indexCount), GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indexCount), GL_UNSIGNED_INT, nullptr);
    }

}

OpenGLMeshBuffer::~OpenGLMeshBuffer()
{
}
