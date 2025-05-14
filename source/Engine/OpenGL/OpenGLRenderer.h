//
// Created by Shaun on 15/03/2025.
//

/**
 * @file OpenGLRenderer.h
 * @brief OpenGLRenderer class which implements rendering via OpenGL.
 * @author Shaun
 * @date 2025-03-15
 */

#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "OpenGLFrameBuffer.h"
#include "OpenGLQuadBuffer.h"
#include "OpenGLShadowMapBuffer.h"
#include "ShaderManager.h"
#include "Interfaces/IDataBuffer.h"
#include "Interfaces/IRenderer.h"
#include "ResourceManagement/Scene.h"

class Texture;

/**
 * @class OpenGLRenderer
 * @brief Implements the IRenderer interface using OpenGL to render scenes, manage shaders, shadow maps, and framebuffers.
 */
class OpenGLRenderer : public IRenderer
{
public:
    /// @brief Constructs the OpenGLRenderer and initializes internal buffers and shader manager.
    OpenGLRenderer();

    /** @brief Renders the provided scene using the current view and projection matrices.
     * @param scene The scene to render.
     * @param viewMatrix The view matrix of the camera.
     * @param projectionMatrix The projection matrix of the camera.
     * @param viewPos The position of the viewer or camera.
     * @return The OpenGL texture ID of the rendered output.
     */
    [[nodiscard]] unsigned int Render(Scene& scene, 
                                      const glm::mat4& viewMatrix,
                                      const glm::mat4& projectionMatrix,
                                      const glm::vec3& viewPos) override;

    /// @brief Clears the OpenGL buffers.
    void Clear() override;

    /// @brief Destructor.
    ~OpenGLRenderer() override;

    /// @brief Gets a const reference to the internal frame buffer.
    /// @return A reference to the OpenGLFrameBuffer.
    [[nodiscard]] const OpenGLFrameBuffer& getFrameBuffer() const { return m_frameBuffer; }

private:
    // Camera removed from here
    unsigned int m_currentShaderID = 0;
    ShaderManager m_shaderManager;
    OpenGLShadowMapBuffer m_shadowMapBuffer;
    OpenGLFrameBuffer m_frameBuffer;
    OpenGLQuadBuffer m_quadBuffer;
    Texture* defaultTexture;

    void LightingPass(Scene& scene, ShaderManager& shaderManager);
    void ShadowPass(Scene& scene, ShaderManager& shaderManager, IDataBuffer& shadowMap);
    void SkyboxPass(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, Scene& scene, ShaderManager& shaderManager);
    void WaterPass(Scene& scene, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& viewPos);
};

#endif //OPENGLRENDERER_H
