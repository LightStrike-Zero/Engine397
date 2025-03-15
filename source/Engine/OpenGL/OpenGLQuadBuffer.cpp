// OpenGLQuadBuffer.cpp
#include "OpenGLQuadBuffer.h"
#include <GL/glew.h>

OpenGLQuadBuffer::OpenGLQuadBuffer() : m_vao(0), m_vbo(0) {
    constexpr float quadVertices[] = {
        // Positions (xy)  // Texture coords (uv)
        -1.0f, -1.0f,      0.0f, 0.0f,
         1.0f, -1.0f,      1.0f, 0.0f,
         1.0f,  1.0f,      1.0f, 1.0f,
        -1.0f, -1.0f,      0.0f, 0.0f,
         1.0f,  1.0f,      1.0f, 1.0f,
        -1.0f,  1.0f,      0.0f, 1.0f
    };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    
    glBindVertexArray(m_vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    
    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    
    // Texture coordinate attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

OpenGLQuadBuffer::~OpenGLQuadBuffer() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void OpenGLQuadBuffer::bind() {
    glBindVertexArray(m_vao);
}

void OpenGLQuadBuffer::unbind() {
    glBindVertexArray(0);
}

void OpenGLQuadBuffer::render() const {
    glDrawArrays(GL_TRIANGLES, 0, 6);
}