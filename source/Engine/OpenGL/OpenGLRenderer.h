//
// Created by Shaun on 15/03/2025.
//

#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H
#include <Camera.h>
#include <entt/entt.hpp>

#include "OpenGLFrameBuffer.h"
#include "OpenGLQuadBuffer.h"
#include "OpenGLShadowMapBuffer.h"
#include "Scene.h"
#include "ShaderManager.h"
#include "Interfaces/IDataBuffer.h"
#include "Interfaces/IRenderer.h"

class Texture;

class OpenGLRenderer : public IRenderer
{
public:
    OpenGLRenderer();
    void Render(Scene& scene) override;
    void Clear() override;
    ~OpenGLRenderer() override;

    const OpenGLFrameBuffer& getFrameBuffer() const { return m_frameBuffer; }

private:
    // TODO why is this still here? it needs to go
    Texture* defaultTexture;
    
    unsigned int m_currentShaderID = 0;
    ShaderManager m_shaderManager;
    OpenGLShadowMapBuffer m_shadowMapBuffer;
    OpenGLFrameBuffer m_frameBuffer;
    OpenGLQuadBuffer m_quadBuffer;
    Camera m_camera;
    
    void ShadowPass(Scene& scene, ShaderManager& shaderManager, IDataBuffer& shadowMap);

    void LightingPass(Scene& scene, ShaderManager& shaderManager);
};

#endif //OPENGLRENDERER_H
