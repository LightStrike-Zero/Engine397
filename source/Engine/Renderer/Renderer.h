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
    void Clear() const;

    void Render(entt::registry& registry, ShaderManager& shaderManager, std::shared_ptr<Shader>& shaderTest);

    // Draws the scene to the shadow map
    void ShadowPass(entt::registry& registry, ShaderManager& shaderManager, IDataBuffer& shadowMap,
                    const glm::mat4& lightSpaceMatrix);

private:
    // why is this still here? it needs to go
    Texture* defaultTexture;

    // used to cache the current shader ID
    unsigned int m_currentShaderID = 0;

    void RenderEntity(entt::registry& registry, entt::entity entity, Shader& shader);
};

#endif //RENDERER_H
