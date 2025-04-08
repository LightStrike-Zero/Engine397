//
// Created by Shaun on 15/03/2025.
//

#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "OpenGLFrameBuffer.h"
#include "OpenGLQuadBuffer.h"
#include "OpenGLShadowMapBuffer.h"
#include "ShaderManager.h"
#include "Interfaces/IDataBuffer.h"
#include "Interfaces/IRenderer.h"
#include "ResourceManagement/NewScene.h"

class Texture;

class OpenGLRenderer : public IRenderer
{
public:
    OpenGLRenderer();
    [[nodiscard]] unsigned int Render(NewScene& scene, 
                                      const glm::mat4& viewMatrix,
                                      const glm::mat4& projectionMatrix,
                                      const glm::vec3& viewPos) override;
    void Clear() override;
    ~OpenGLRenderer() override;

    [[nodiscard]] const OpenGLFrameBuffer& getFrameBuffer() const { return m_frameBuffer; }

private:
    // Camera removed from here
    unsigned int m_currentShaderID = 0;
    ShaderManager m_shaderManager;
    OpenGLShadowMapBuffer m_shadowMapBuffer;
    OpenGLFrameBuffer m_frameBuffer;
    OpenGLQuadBuffer m_quadBuffer;
    Texture* defaultTexture;
    

    void LightingPass(NewScene& scene, ShaderManager& shaderManager);
    void ShadowPass(NewScene& scene, ShaderManager& shaderManager, IDataBuffer& shadowMap);
};

#endif //OPENGLRENDERER_H
