//
// Created by Shaun on 15/03/2025.
//

/**
 * @file IRenderer.h
 * @brief Interface for rendering components.
 * Declares an abstract renderer that can render a scene with camera and view information.
 * @author Shaun
 * @date 2025-03-15
 */

#ifndef IRENDERER_H
#define IRENDERER_H
#include <glm/fwd.hpp>

#include "ResourceManagement/Scene.h"

/**
 * @class IRenderer
 * @brief Abstract base class for scene renderers.
 */
class IRenderer
{
public:
    /**
     * @brief Default constructor.
     */
    IRenderer() = default;

    /**
     * @brief Renders a scene with the given view and projection matrices.
     * @param scene The scene to render.
     * @param viewMatrix The view matrix (camera).
     * @param projectionMatrix The projection matrix.
     * @param viewPos The camera/view position in world space.
     * @return Texture ID or framebuffer ID of the rendered output.
     */
    virtual unsigned int Render(Scene& scene, 
                               const glm::mat4& viewMatrix, 
                               const glm::mat4& projectionMatrix, 
                               const glm::vec3& viewPos) = 0;

    /**
     * @brief Clears the rendering target (e.g., framebuffer).
     */
    virtual void Clear() = 0;

    /**
     * @brief Virtual destructor.
     */
    virtual ~IRenderer() = default;

};

#endif //IRENDERER_H