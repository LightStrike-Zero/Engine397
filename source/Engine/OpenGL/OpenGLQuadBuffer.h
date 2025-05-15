//
// Created by Shaun on 15/03/2025.
//

/**
 * @file OpenGLQuadBuffer.h
 * @brief Declares a buffer specifically for rendering 2D screen-space quads using OpenGL.
 * @author Shaun
 * @date 2025-03-15
 */

#ifndef OPENGLQUADBUFFER_H
#define OPENGLQUADBUFFER_H

#include "Interfaces/IDataBuffer.h"

/**
 * @class OpenGLQuadBuffer
 * @brief An OpenGL implementation of a data buffer used for rendering fullscreen or 2D quads.
 */
class OpenGLQuadBuffer : public IDataBuffer
{
public:
    /**
     * @brief Constructor. Initializes VAO and VBO for the quad.
     */
    OpenGLQuadBuffer();

    /**
     * @brief Destructor. Cleans up OpenGL resources.
     */
    ~OpenGLQuadBuffer() override;


    /**
     * @brief Binds the buffer to the current OpenGL context.
     */
    void bind() override;

    /**
     * @brief Unbinds the buffer from the current OpenGL context.
     */
    void unbind() override;

    /**
     * @brief Renders the quad using the currently bound shader and state.
     */
    void render() const;

    /**
     * @author buko
     * @brief initialises buffer
     */
    void initialise();

private:
    unsigned int m_vao, m_vbo;
    
};

#endif //OPENGLQUADBUFFER_H
