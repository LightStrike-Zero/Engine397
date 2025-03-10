#include "Renderer.h"
#include <GL/glew.h>
#include <iostream>
#include <TextureManager.h>
#include <glm/ext/matrix_transform.hpp>

#include "TextureLoader.h"
#include "Components/MaterialComponent.h"
#include "Components/RenderableComponent.h"
#include "Components/TransformComponent.h"


Renderer::Renderer()
{
    // Load the default texture
    defaultTexture = new Texture("Assets/default/error.jpg");
    if (defaultTexture)
    {
        // std::cout << "Default texture loaded successfully." << std::endl;
    }
    else
    {
        std::cerr << "Failed to load default texture." << std::endl;
    }
}

Renderer::~Renderer()
{
    delete defaultTexture;
}

void Renderer::Clear() const
{
    // non API agnostic
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Render(entt::registry& registry, ShaderManager& shaderManager, std::shared_ptr<Shader>& shadertest)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Iterate over entities with Mesh, Transform, and Material components
    auto view = registry.view<RenderableComponent, TransformComponent, MaterialComponent>();
    for (auto entity : view) {
        auto& mesh = registry.get<RenderableComponent>(entity);
        auto& transform = registry.get<TransformComponent>(entity);
        auto& material = registry.get<MaterialComponent>(entity);

        auto shader = shaderManager.getShader(material.shaderID);
        // essentially we just want to check if the currently bound shader is the same as the shader we want to use
        if (shader->GetShaderID() != m_currentShaderID)
        {
            shader->Bind();
            m_currentShaderID = shader->GetShaderID();
        }

        glm::mat4 modelMatrix = transform.getModelMatrix();
        shader->SetUniformMat4f("model", modelMatrix);

        // check for decals and enable transparency CURRENTLY JUST NOT RENDERING DECAL MESHES
        if (!material.isDecal)
        {
            // if (!material.isDecal)
            // {
            //     glEnable(GL_BLEND);
            //     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            // } else
            // {
            //     glDisable(GL_BLEND);
            // }

            // Bind the diffuse texture
            if (material.baseColorTextureID != 0) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, material.baseColorTextureID);
                shader->SetUniform1i("albedoMap", 0);
            } else {
                defaultTexture->bind(0);
                shader->SetUniform1i("albedoMap", 0);
            }
            if (material.normalTextureID != 0)
            {
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, material.normalTextureID);
                shader->SetUniform1i("normalMap", 1);
            }
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, material.roughnessTextureID);
            shader->SetUniform1i("roughnessMap", 2);

            mesh.meshBuffer->bind();
            mesh.meshBuffer->draw();
            glBindVertexArray(0);
            // dont unbind the shader, since we are LIKELY to use it again
        }
    }

}
// TODO this is the old render code, i kept it for reference
// void Renderer::Render(entt::registry& registry, ShaderManager& shaderManager, std::shared_ptr<Shader>& shadertest)
// {
//     glEnable(GL_CULL_FACE);
//     glCullFace(GL_BACK);
//
//     // Iterate over entities with Mesh, Transform, and Material components
//     auto view = registry.view<RenderableComponent, TransformComponent, MaterialComponent>();
//     for (auto entity : view) {
//         auto& mesh = registry.get<RenderableComponent>(entity);
//         auto& transform = registry.get<TransformComponent>(entity);
//         auto& material = registry.get<MaterialComponent>(entity);
//
//         auto shader = shaderManager.getShader(material.shaderID);
//         // essentially we just want to check if the currently bound shader is the same as the shader we want to use
//         if (shader->GetShaderID() != m_currentShaderID)
//         {
//             shader->Bind();
//             m_currentShaderID = shader->GetShaderID();
//         }
//
//         glm::mat4 modelMatrix = transform.getModelMatrix();
//         shader->SetUniformMat4f("model", modelMatrix);
//
//         // check for decals and enable transparency CURRENTLY JUST NOT RENDERING DECAL MESHES
//         if (!material.isDecal)
//         {
//             // if (!material.isDecal)
//             // {
//             //     glEnable(GL_BLEND);
//             //     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//             // } else
//             // {
//             //     glDisable(GL_BLEND);
//             // }
//
//             // Bind the diffuse texture
//             if (material.baseColorTextureID != 0) {
//                 glActiveTexture(GL_TEXTURE0);
//                 glBindTexture(GL_TEXTURE_2D, material.baseColorTextureID);
//                 shader->SetUniform1i("albedoMap", 0);
//             } else {
//                 defaultTexture->bind(0);
//                 shader->SetUniform1i("albedoMap", 0);
//             }
//             if (material.normalTextureID != 0)
//             {
//                 glActiveTexture(GL_TEXTURE1);
//                 glBindTexture(GL_TEXTURE_2D, material.normalTextureID);
//                 shader->SetUniform1i("normalMap", 1);
//             }
//             glActiveTexture(GL_TEXTURE2);
//             glBindTexture(GL_TEXTURE_2D, material.roughnessTextureID);
//             shader->SetUniform1i("roughnessMap", 2);
//
//
//             glBindVertexArray(mesh.vao);
//             glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.indexCount), GL_UNSIGNED_INT, nullptr);
//             glBindVertexArray(0);
//             // dont unbind the shader, since we are LIKELY to use it again
//         }
//     }
//
// }

void Renderer::ShadowPass(entt::registry& registry, ShaderManager& shaderManager, ShadowMap& shadowMap, const glm::mat4& lightSpaceMatrix)
{
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glCullFace(GL_FRONT); // Avoid shadow acne

    shadowMap.Bind();
    glClear(GL_DEPTH_BUFFER_BIT);



    // Iterate over entities with Mesh and Transform components
    auto view = registry.view<RenderableComponent, TransformComponent>();
    for (auto entity : view) {
        auto& mesh = registry.get<RenderableComponent>(entity);
        auto& transform = registry.get<TransformComponent>(entity);
        auto& material = registry.get<MaterialComponent>(entity);

        auto shadowShader = shaderManager.getShader("shadowShader"); // for now we'll hard code this, remove it from main.
        if (shadowShader->GetShaderID() != m_currentShaderID)
        {
            shadowShader->Bind();
            m_currentShaderID = shadowShader->GetShaderID();

        }
        shadowShader->SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);

        // check for decals and enable transparency CURRENTLY JUST NOT RENDERING DECAL MESHES
        if (!material.isDecal)
        {
            glm::mat4 modelMatrix = transform.getModelMatrix();
            shadowShader->SetUniformMat4f("model", modelMatrix);

            mesh.meshBuffer->bind();
            mesh.meshBuffer->draw();
            glBindVertexArray(0);
        }
    }


    // shadowShader->Unbind();
    shadowMap.Unbind();
    glCullFace(GL_BACK); // Reset to default cull face
    glDepthMask(GL_TRUE);
}


void Renderer::LightingPass(entt::registry& registry, Shader& shader) const {
    glCullFace(GL_BACK);
    shader.Bind();

    // Iterate over entities with Mesh, Transform, and Material components
    auto view = registry.view<RenderableComponent, TransformComponent, MaterialComponent>();
    for (auto entity : view) {
        auto& mesh = registry.get<RenderableComponent>(entity);
        auto& transform = registry.get<TransformComponent>(entity);
        auto& material = registry.get<MaterialComponent>(entity);

        glm::mat4 modelMatrix = transform.getModelMatrix();
        shader.SetUniformMat4f("model", modelMatrix);

        // Bind the diffuse texture
        if (material.baseColorTextureID != 0) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, material.baseColorTextureID);
            shader.SetUniform1i("material.diffuse", 0);
        } else {
            defaultTexture->bind(0);
            shader.SetUniform1i("material.diffuse", 0);
        }
        if (material.normalTextureID != 0)
        {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, material.normalTextureID);
            shader.SetUniform1i("normalMap", 1);
        }

        mesh.meshBuffer->bind();
        mesh.meshBuffer->draw();
        glBindVertexArray(0);
    }

    shader.Unbind();
}