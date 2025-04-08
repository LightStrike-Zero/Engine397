//
// Created by Shaun on 15/03/2025.
//

#ifndef IRENDERER_H
#define IRENDERER_H
#include <glm/fwd.hpp>

#include "ResourceManagement/NewScene.h"

class IRenderer
{
public:
    IRenderer() = default;
    virtual unsigned int Render(NewScene& scene, 
                               const glm::mat4& viewMatrix, 
                               const glm::mat4& projectionMatrix, 
                               const glm::vec3& viewPos) = 0;
    virtual void Clear() = 0;
    virtual ~IRenderer() = default;

};


#endif //IRENDERER_H
