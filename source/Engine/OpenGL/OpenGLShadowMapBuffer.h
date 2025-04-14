//
// Created by Shaun on 15/03/2025.
//

/**
 * @file OpenGLShadowMapBuffer.h
 * @brief Defines the OpenGL implementation for a shadow map framebuffer object.
 * @author Shaun
 * @date 2025-03-15
 */

#ifndef OPENGLSHADOWMAPBUFFER_H
#define OPENGLSHADOWMAPBUFFER_H
#include <GL/glew.h>
#include <glm/fwd.hpp>

#include "Interfaces/IDataBuffer.h"

/**
 * @class OpenGLShadowMapBuffer
 * @brief Implements a shadow map depth buffer using OpenGL framebuffer objects.
 */
class OpenGLShadowMapBuffer : public IDataBuffer {
public:
    /**
     * @brief Constructs the shadow map buffer with specified dimensions.
     * @param width The width of the shadow map.
     * @param height The height of the shadow map.
     */
    OpenGLShadowMapBuffer(unsigned int width, unsigned int height);

    /**
     * @brief Destructor. Cleans up OpenGL resources.
     */
    ~OpenGLShadowMapBuffer() override;

    /**
     * @brief Binds the framebuffer for rendering the shadow map.
     */
    void bind() override;

    /**
     * @brief Unbinds the framebuffer and restores the default render target.
     */
    void unbind() override;

    /**
     * @brief Retrieves the OpenGL texture ID of the depth map.
     * @return The texture ID used for the shadow map.
     */
    [[nodiscard]] GLuint GetDepthTexture() const;

    /**
     * @brief Calculates a light space transformation matrix for directional shadows.
     * @param lightDirection The direction vector of the light source.
     * @param playerPosition The position of the player or camera in world space.
     * @return The orthographic projection-view matrix from the light's perspective.
     */
    static glm::mat4 CalculateLightSpaceMatrix(const glm::vec3& lightDirection, const glm::vec3& playerPosition);

private:
    GLuint m_shadowMapBuffer;
    GLuint m_textureID;
    unsigned int width, height;
};

#endif //OPENGLSHADOWMAPBUFFER_H
