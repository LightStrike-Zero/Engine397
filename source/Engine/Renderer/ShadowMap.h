//
// Created by Shaun on 7/12/2024.
//

#ifndef SHADOWMAP_H
#define SHADOWMAP_H
#include <GL/glew.h>
#include <glm/fwd.hpp>

class ShadowMap {
public:
    ShadowMap(unsigned int width, unsigned int height);
    ~ShadowMap();

    void Bind();
    void Unbind();
    GLuint GetDepthTexture() const;

    glm::mat4 CalculateLightSpaceMatrix(const glm::vec3& lightDirection);

private:
    GLuint shadowMapFBO;
    GLuint shadowMapTexture;
    unsigned int width, height;
};

#endif //SHADOWMAP_H
