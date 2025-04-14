//
// Created by Shaun on 15/03/2025.
//

#include "OpenGLShadowMapBuffer.h"


/**
 * @file OpenGLShadowMapBuffer.cpp
 * @brief Implementation of the OpenGLShadowMapBuffer class.
 *
 * @author Shaun
 * @date Created: 7/12/2024
 * @date Updated: 15/03/2025
 */

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
OpenGLShadowMapBuffer::OpenGLShadowMapBuffer(unsigned int width, unsigned int height)
    : m_shadowMapBuffer(0), m_textureID(0), width(width), height(height) {
    // Generate the framebuffer
    glGenFramebuffers(1, &m_shadowMapBuffer);

    // Generate the depth texture
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // **Uncomment and set the border color to white**
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Attach the depth texture to the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_textureID, 0);
    glDrawBuffer(GL_NONE); // No color buffer is drawn
    glReadBuffer(GL_NONE);

    // Check framebuffer status
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ShadowMap framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind the framebuffer
}

OpenGLShadowMapBuffer::~OpenGLShadowMapBuffer() {
    glDeleteFramebuffers(1, &m_shadowMapBuffer);
    glDeleteTextures(1, &m_textureID);
}




void OpenGLShadowMapBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapBuffer);
    glViewport(0, 0, width, height); // Set viewport size to shadow map resolution
}

void OpenGLShadowMapBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind the framebuffer
}






GLuint OpenGLShadowMapBuffer::GetDepthTexture() const {
    return m_textureID;
}

glm::mat4 OpenGLShadowMapBuffer::CalculateLightSpaceMatrix(const glm::vec3& lightDirection, const glm::vec3& playerPosition) {
    glm::vec3 normalizedLightDir = glm::normalize(lightDirection);

    float orthoSize = 800.0f; // Adjust based on how much area around the player to cover
    glm::mat4 lightProjection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, 1.0f, 500.0f);

    float distanceFromPlayer = 200.0f;
    glm::vec3 lightPosition = playerPosition - normalizedLightDir * distanceFromPlayer;

    glm::mat4 lightView = glm::lookAt(lightPosition, playerPosition, glm::vec3(0.0f, 1.0f, 0.0f));

    return lightProjection * lightView;
}