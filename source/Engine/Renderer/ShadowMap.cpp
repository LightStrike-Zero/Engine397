/**
 * @file ShadowMap.cpp
 * @brief Implementation of the ShadowMap class.
 *
 * @author Shaun
 * @date Created: 7/12/2024
 */

#include "ShadowMap.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
ShadowMap::ShadowMap(unsigned int width, unsigned int height)
    : shadowMapFBO(0), shadowMapTexture(0), width(width), height(height) {
    // Generate the framebuffer
    glGenFramebuffers(1, &shadowMapFBO);

    // Generate the depth texture
    glGenTextures(1, &shadowMapTexture);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
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
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTexture, 0);
    glDrawBuffer(GL_NONE); // No color buffer is drawn
    glReadBuffer(GL_NONE);

    // Check framebuffer status
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ShadowMap framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind the framebuffer
}

ShadowMap::~ShadowMap() {
    glDeleteFramebuffers(1, &shadowMapFBO);
    glDeleteTextures(1, &shadowMapTexture);
}

void ShadowMap::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glViewport(0, 0, width, height); // Set viewport size to shadow map resolution
}

void ShadowMap::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind the framebuffer
}

GLuint ShadowMap::GetDepthTexture() const {
    return shadowMapTexture;
}

glm::mat4 ShadowMap::CalculateLightSpaceMatrix(const glm::vec3& lightDirection) {
    // Normalize the light direction
    glm::vec3 normalizedLightDir = glm::normalize(lightDirection);

    // Define a fixed orthographic projection for the shadow map
    float orthoSize = 15.0f; // Adjust based on your scene's size
    glm::mat4 lightProjection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, 1.0f, 50.0f);
    // glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);


    // Calculate the light's view matrix
    // Position the light sufficiently far to cover the scene
    glm::mat4 lightView = glm::lookAt(-normalizedLightDir * 30.0f, // Increased distance for broader coverage
                                      glm::vec3(0.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f));

    // Combine projection and view matrices
    return lightProjection * lightView;
}