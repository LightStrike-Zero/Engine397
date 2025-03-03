//
// Created by Shaun on 29/01/2025.
//

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

namespace Carbon
{
    class FrameBuffer
    {
    public:
        FrameBuffer(const unsigned int width, const unsigned int height);
        ~FrameBuffer();

        void Bind();
        void Unbind();

        unsigned int GetTextureColorBuffer() const { return m_textureColorBuffer; }

    private:
        unsigned int m_frameBuffer;
        unsigned int m_renderBuffer;
        unsigned int m_textureColorBuffer;
        unsigned int m_width, m_height = 0;

    };
} // Carbon

#endif //FRAMEBUFFER_H
