// OpenGLQuadBuffer.cpp
#include "OpenGLQuadBuffer.h"

#include <iostream>
#include <GL/glew.h>

OpenGLQuadBuffer::OpenGLQuadBuffer() {
    float quad[] = {
        // First Triangle
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        // Second Triangle
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 1.0f
    };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    
    glBindVertexArray(m_vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    
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
    glBindVertexArray(m_vao); // added by buko
    glDrawArrays(GL_TRIANGLES, 0, 6);  // this line was alreayd here
    glBindVertexArray(0); // added by buko
}

// buko ---------------------------------------------
void OpenGLQuadBuffer::initialise()
{
    float vertices[] = {
            // positions   // texCoords
            0.0f, 1.0f,    0.0f, 1.0f,
            1.0f, 0.0f,    1.0f, 0.0f,
            0.0f, 0.0f,    0.0f, 0.0f,

            0.0f, 1.0f,    0.0f, 1.0f,
            1.0f, 1.0f,    1.0f, 1.0f,
            1.0f, 0.0f,    1.0f, 0.0f
    };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position (2 floats)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture Coordinates (2 floats)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}