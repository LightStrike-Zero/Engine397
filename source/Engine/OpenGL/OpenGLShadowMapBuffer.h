//
// Created by Shaun on 15/03/2025.
//

#ifndef OPENGLSHADOWMAPBUFFER_H
#define OPENGLSHADOWMAPBUFFER_H
#include <GL/glew.h>
#include <glm/fwd.hpp>

#include "Interfaces/IDataBuffer.h"

class OpenGLShadowMapBuffer : public IDataBuffer {
public:
    OpenGLShadowMapBuffer(unsigned int width, unsigned int height);
    ~OpenGLShadowMapBuffer() override;

    void bind() override;
    void unbind() override;
    [[nodiscard]] GLuint GetDepthTexture() const;

    static glm::mat4 CalculateLightSpaceMatrix(const glm::vec3& lightDirection);

private:
    GLuint m_shadowMapBuffer;
    GLuint m_textureID;
    unsigned int width, height;
};


#endif //OPENGLSHADOWMAPBUFFER_H
