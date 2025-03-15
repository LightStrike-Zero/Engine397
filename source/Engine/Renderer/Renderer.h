#ifndef RENDERER_H
#define RENDERER_H


#include <entt/entt.hpp>

#include "../Actors/ModelData.h" // Include the new ModelData structures
#include "Shader.h"             // Forward declare the Shader class
#include "ShaderManager.h"
#include "ShadowMap.h"
#include "Importers/AssimpImporter.h"


class IDataBuffer;

class Renderer
{
public:
    Renderer();
    ~Renderer();
    // Clear the screen
    void Clear() const;

    // Render a single ModelData object
    void Render(entt::registry& registry, ShaderManager& shaderManager, std::shared_ptr<Shader>& shaderTest);
    // Render multiple ModelData objects
    // void Render(const std::vector<ModelData>& models, Shader& shader) const;

    // Draws the scene to the shadow map
    void ShadowPass(entt::registry& registry, ShaderManager& shaderManager, IDataBuffer& shadowMap,
                    const glm::mat4& lightSpaceMatrix);
    // render the final scene including the shadow map
    void LightingPass(entt::registry& registry, Shader& shader) const;

private:
    // why is this still here? it needs to go
    Texture* defaultTexture;

    // used to cache the current shader ID
    unsigned int m_currentShaderID = 0;

    void RenderEntity(entt::registry& registry, entt::entity entity, Shader& shader);
};

#endif //RENDERER_H
