//
// Created by Shaun on 15/03/2025.
//

/**
 * @file OpenGLFrameBuffer.h
 * @brief OpenGLFrameBuffer class used for off-screen rendering using framebuffers.
 *
 * This class implements an OpenGL-based framebuffer, providing colour attachment and depth buffering.
 * It conforms to the IDataBuffer interface.
 * @author Shaun
 * @date 2025-03-15
 */

#ifndef OPENGLFRAMEBUFFER_H
#define OPENGLFRAMEBUFFER_H

#include "Interfaces/IDataBuffer.h"

/**
 * @class OpenGLFrameBuffer
 * @brief Represents an OpenGL framebuffer object (FBO) for off-screen rendering.
 */
class OpenGLFrameBuffer : public IDataBuffer 
{
  public:

    /**
     * @brief Constructs an OpenGL framebuffer with the specified dimensions.
     * @param width Width of the framebuffer in pixels.
     * @param height Height of the framebuffer in pixels.
     */
    OpenGLFrameBuffer(unsigned int width, unsigned int height);

    /**
     * @brief Destroys the framebuffer and releases its OpenGL resources.
     */
    ~OpenGLFrameBuffer() override;

    /**
     * @brief Binds the framebuffer
     */
    void bind() override;

    /**
     * @brief Unbinds the framebuffer
     */
    void unbind() override;

    /**
     * @brief Retrieves the texture colour buffer attached to the framebuffer.
     * @return OpenGL texture ID of the colour buffer.
     */
    [[nodiscard]] unsigned int getTextureColorBuffer() const { return m_textureColorBuffer; }
    
    private:
        unsigned int m_frameBuffer;                ///< OpenGL framebuffer object ID
        unsigned int m_renderBuffer;               ///< OpenGL renderbuffer object ID (for depth/stencil)
        unsigned int m_textureColorBuffer;         ///< Texture ID used as the color buffer
        unsigned int m_width, m_height = 0;        ///< Dimensions of the framebuffer

};

#endif //OPENGLFRAMEBUFFER_H