//
// Created by Shaun on 15/03/2025.



#include "OpenGLRenderer.h"

#include <iostream>
#include <Components/RenderableComponent.h>
#include <GL/glew.h>

#include "OpenGLShadowMapBuffer.h"

#include "Components/MaterialComponent.h"
#include "Components/SkyboxComponent.h"
#include "Components/TransformComponent.h"
#include "ResourceManagement/Scene.h"
#include "texture/TextureLoader.h"

OpenGLRenderer::OpenGLRenderer()
    :  m_shaderManager(ShaderManager()),
        m_shadowMapBuffer(OpenGLShadowMapBuffer(4096, 4096)), m_frameBuffer(OpenGLFrameBuffer(2048, 2048)), m_quadBuffer(OpenGLQuadBuffer())
{

    m_shaderManager.loadShader("lightingShader", "Vert.glsl", "Frag.glsl");
    m_shaderManager.loadShader("skyboxShader", "skybox_vert.glsl", "skybox_frag.glsl");
    m_shaderManager.loadShader("shadowShader", "shadow_vertex.glsl", "shadow_fragment.glsl");
    m_shaderManager.loadShader("framebufferShader", "Frame_Vert.glsl", "Frame_Frag.glsl");

}

OpenGLRenderer::~OpenGLRenderer()
{
}

void OpenGLRenderer::Clear() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


unsigned int OpenGLRenderer::Render(Scene& scene, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& viewPos)
{
    const auto& lightingShader = m_shaderManager.getShader("lightingShader");
    const auto& framebufferShader = m_shaderManager.getShader("framebufferShader");

    ShadowPass(scene, m_shaderManager, m_shadowMapBuffer); // 4
    m_frameBuffer.bind();
    
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
    glEnable(GL_DEPTH_TEST);

    SkyboxPass(viewMatrix, projectionMatrix, scene, m_shaderManager);
    glm::vec3 playerPos;
    auto playerView = scene.getEntityManager().view<TransformComponent, PlayerControllerComponent>();
    for (auto entity : playerView)
    {
        auto& playerTransform = playerView.get<TransformComponent>(entity);
        playerPos = playerTransform.position;
    }
        glm::mat4 lightSpaceMatrix = OpenGLShadowMapBuffer::CalculateLightSpaceMatrix(scene.getDirectionalLight().getDirection(), playerPos);
    lightingShader->bind(); //1
    m_currentShaderID = lightingShader->getID();
    lightingShader->SetUniformMat4("view", viewMatrix);
    lightingShader->SetUniformMat4("projection", projectionMatrix);
    lightingShader->SetUniform3f("viewPos", viewPos);

    lightingShader->SetUniform3f("light.direction", scene.getDirectionalLight().getDirection());
    lightingShader->SetUniform3f("light.ambient", scene.getDirectionalLight().getAmbient());
    lightingShader->SetUniform3f("light.diffuse", scene.getDirectionalLight().getDiffuse());
    lightingShader->SetUniform3f("light.specular", scene.getDirectionalLight().getSpecular());
    lightingShader->SetUniformMat4("lightSpaceMatrix", lightSpaceMatrix);
    
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, m_shadowMapBuffer.GetDepthTexture());
    lightingShader->SetUniform1i("shadowMap", 4);
    lightingShader->SetUniform2f("gMapSize", glm::vec2(4096.0f, 4096.0f));
    
    LightingPass(scene, m_shaderManager); //1

    m_frameBuffer.unbind();

    
    framebufferShader->bind(); //7
    m_currentShaderID = framebufferShader->getID();
    m_quadBuffer.bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_frameBuffer.getTextureColorBuffer());
    framebufferShader->SetUniform1i("framebuf", 0);
    m_quadBuffer.render();
    m_quadBuffer.unbind();

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error: " << error << std::endl;
    }
    return m_frameBuffer.getTextureColorBuffer();
    
}
 
void OpenGLRenderer::LightingPass(Scene& scene, ShaderManager& shaderManager)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Iterate over entities with Mesh, Transform, and Material components
    auto view = scene.getEntityManager().view<RenderableComponent, TransformComponent, MaterialComponent>();
    for (auto entity : view) {
        auto& mesh = scene.getEntityManager().get<RenderableComponent>(entity);
        auto& transform = scene.getEntityManager().get<TransformComponent>(entity);
        auto& material = scene.getEntityManager().get<MaterialComponent>(entity);
        
        auto shader = shaderManager.getShader(material.shaderID);
        // essentially we just want to check if the currently bound shader is the same as the shader we want to use
        if (shader->getID() != m_currentShaderID)
        {
            shader->bind();
            m_currentShaderID = shader->getID();
        }

        glm::mat4 modelMatrix = transform.getModelMatrix();
        shader->SetUniformMat4("model", modelMatrix);

        if (!material.isDecal)
        {

            // Bind the diffuse texture
            if (material.baseColorTextureID != 0) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, material.baseColorTextureID);
                shader->SetUniform1i("albedoMap", 0);
                shader->SetUniform1i("hasAlbedoMap", 1);
            } else {
                shader->SetUniform1i("hasAlbedoMap", 0);
                shader->SetUniform3f("fallbackColor", {1.0f, 0.0f, 0.5f});
            }
            if (material.normalTextureID != 0)
            {
                // glActiveTexture(GL_TEXTURE1);
                // glBindTexture(GL_TEXTURE_2D, material.normalTextureID);
                // shader->SetUniform1i("normalMap", 1);
            }
            // glActiveTexture(GL_TEXTURE2);
            // glBindTexture(GL_TEXTURE_2D, material.roughnessTextureID);
            // shader->SetUniform1i("roughnessMap", 2);

            mesh.meshBuffer->bind();
            mesh.meshBuffer->draw();
            glBindVertexArray(0);
            // don't unbind the shader, since we are LIKELY to use it again
        }
    }

}


void OpenGLRenderer::SkyboxPass(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, Scene& scene, ShaderManager& shaderManager)
{
    glDepthMask(GL_FALSE);

    // Retrieve and bind the skybox shader.
    auto skyboxShader = shaderManager.getShader("skyboxShader");
    skyboxShader->bind();
    m_currentShaderID = skyboxShader->getID();

    // Remove translation from the view matrix.
    glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(viewMatrix));
    skyboxShader->SetUniformMat4("view", viewNoTranslation);
    skyboxShader->SetUniformMat4("projection", projectionMatrix);

    // Iterate over all entities that have the SkyboxComponent, RenderableComponent, and TransformComponent.
    auto skyboxView = scene.getEntityManager().view<SkyboxComponent, RenderableComponent, TransformComponent>();
    for (auto entity : skyboxView) {
        auto& skyboxComp = skyboxView.get<SkyboxComponent>(entity);
        auto& renderable = skyboxView.get<RenderableComponent>(entity);
        auto& transform = skyboxView.get<TransformComponent>(entity);

        // Bind the cubemap texture to texture unit 0.
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxComp.cubemapID);
        skyboxShader->SetUniform1i("skybox", 0);

        // In many skybox shaders, the model transform is not used (or assumed to be identity).
        // If your implementation uses a transform, you can pass it here.
        skyboxShader->SetUniformMat4("model", transform.getModelMatrix());

        // Draw the skybox geometry.
        renderable.meshBuffer->bind();
        renderable.meshBuffer->draw();
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);
    }

    // Restore default depth function.
    glDepthFunc(GL_LESS);
}

void OpenGLRenderer::ShadowPass(Scene& scene, ShaderManager& shaderManager, IDataBuffer& shadowMap)
{
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glCullFace(GL_FRONT); // Avoid shadow acne

    shadowMap.bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    
    // Iterate over entities with Mesh and Transform components but exclude SkyboxComponent
    auto view = scene.getEntityManager().view<RenderableComponent, TransformComponent>(exclude<SkyboxComponent>);
    for (auto entity : view) {
        auto& mesh = scene.getEntityManager().get<RenderableComponent>(entity);
        auto& transform = scene.getEntityManager().get<TransformComponent>(entity);

        auto shadowShader = shaderManager.getShader("shadowShader"); // for now we'll hard code this
        if (shadowShader->getID() != m_currentShaderID)
        {
            shadowShader->bind();
            m_currentShaderID = shadowShader->getID();

        }
        auto playerView = scene.getEntityManager().view<TransformComponent, PlayerControllerComponent>();
        for (auto entity : playerView) {
            auto& playerTransform = playerView.get<TransformComponent>(entity);
            glm::vec3 playerPos = playerTransform.position;
            shadowShader->SetUniformMat4("lightSpaceMatrix", OpenGLShadowMapBuffer::CalculateLightSpaceMatrix(scene.getDirectionalLight().getDirection(), playerPos));
        }
            
        

            glm::mat4 modelMatrix = transform.getModelMatrix();
            shadowShader->SetUniformMat4("model", modelMatrix);

            mesh.meshBuffer->bind();
            mesh.meshBuffer->draw();
            glBindVertexArray(0);
    }
    shadowMap.unbind();
    glCullFace(GL_BACK); // Reset to default cull face
    glDepthMask(GL_TRUE);
}
