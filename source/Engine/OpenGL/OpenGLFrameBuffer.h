//
// Created by Shaun on 15/03/2025.
//


#ifndef OPENGLFRAMEBUFFER_H
#define OPENGLFRAMEBUFFER_H

#include "Interfaces/IDataBuffer.h"

class OpenGLFrameBuffer : public IDataBuffer 
{
  public:
    OpenGLFrameBuffer(unsigned int width, unsigned int height);
    ~OpenGLFrameBuffer() override;

    void bind() override;
    void unbind() override;

    [[nodiscard]] unsigned int getTextureColorBuffer() const { return m_textureColorBuffer; }
    
    private:
        unsigned int m_frameBuffer;
        unsigned int m_renderBuffer;
        unsigned int m_textureColorBuffer;
        unsigned int m_width, m_height = 0;

};


#endif //OPENGLFRAMEBUFFER_H
