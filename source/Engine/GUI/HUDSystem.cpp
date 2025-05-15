//
// Created by buko on 15/05/2025.
//

#include "HUDSystem.h"
#include "Interfaces/IShader.h"
#include "Texture/TextureManager.h"
#include "OpenGL/OpenGLQuadBuffer.h"
#include "Texture/TextureManager.h"

#include <GL/glew.h>
#include <iostream>

HUDSystem::HUDSystem()
{
    m_shader = nullptr;
    m_quad = nullptr;
}

void HUDSystem::initialise(IShader* shader, std::shared_ptr<OpenGLQuadBuffer> quad)
{
    m_shader = shader;
    m_quad = std::move(quad);
}

void HUDSystem::addHUDItem(const std::string& name, const std::string& texturePath, const glm::vec2& position, const glm::vec2& size)
{
    uint32_t textureID = TextureManager::getInstance().loadTextureFromFile(texturePath);
    m_items.push_back({name, textureID, position, size});
}

void HUDSystem::renderAll()
{
    std::cout << "[HUDSystem] renderAll() called, item count: " << m_items.size() << "\n";

    if (!m_shader || !m_quad) {
        std::cerr << "[HUDSystem] Error: Shader or Quad buffer not initialised!" << std::endl;
        return;
    }

    m_shader->bind();

    glm::vec2 screenSize(1920.0f, 1080.0f); // ❗ Update if this should be dynamic
    m_shader->SetUniform2f("u_ScreenSize", screenSize);

    for (const auto& item : m_items)
    {
        m_shader->SetUniform2f("u_Position", item.position);
        m_shader->SetUniform2f("u_Size", item.size);
        m_shader->SetUniform1i("u_Texture", 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, item.textureID);

        m_quad->render();
    }

    m_shader->unbind();
}
