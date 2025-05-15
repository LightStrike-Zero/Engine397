//
// Created by buko on 15/05/2025.
//

/**
 * @file HUDSystem.h
 * @brief Provides functionality for rendering simple 2D HUD elements such as icons, images, and overlays using textured quads.
 *
 * This HUD system renders via an orthographic projection.
 * This projection will put elements in the screen space and not the world space
 */

#ifndef HUDSYSTEM_H
#define HUDSYSTEM_H

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

// Forward declarations
class IShader;
class OpenGLQuadBuffer;

/**
 * Represents a single HUD element (name, texture ID, position, size).
 */
struct HUDElement{
    std::string name;
    uint32_t textureID;
    glm::vec2 position;
    glm::vec2 size;
};

/**
 * A retained-mode 2D HUD system using textured quads and screen-space coordinates.
 */
class HUDSystem
{
public:
    HUDSystem();

    void initialise(IShader* shader, std::shared_ptr<OpenGLQuadBuffer> quad);
    void addHUDItem(const std::string& name, const std::string& texturePath, const glm::vec2& position, const glm::vec2& size);
    void renderAll();

private:
    IShader* m_shader;
    std::shared_ptr<OpenGLQuadBuffer> m_quad;
    std::vector<HUDElement> m_items;
};

#endif // HUDSYSTEM_H
