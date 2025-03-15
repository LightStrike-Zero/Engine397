//
// Created by Shaun on 15/03/2025.
//

#ifndef OPENGLQUADBUFFER_H
#define OPENGLQUADBUFFER_H

#include "Interfaces/IDataBuffer.h"

class OpenGLQuadBuffer : public IDataBuffer
{
public:
    OpenGLQuadBuffer();
    ~OpenGLQuadBuffer() override;

    void bind() override;
    void unbind() override;
    void render() const;
private:
    unsigned int m_vao, m_vbo;
    
};

#endif //OPENGLQUADBUFFER_H
