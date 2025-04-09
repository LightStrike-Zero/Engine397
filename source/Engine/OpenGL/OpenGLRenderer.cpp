//
// Created by Shaun on 15/03/2025.
//


#include "OpenGLRenderer.h"

#include <iostream>
#include <Components/RenderableComponent.h>
// #include <entt/entt.hpp>
#include <GL/glew.h>

#include "OpenGLShadowMapBuffer.h"
// #include "Scene.h"
#include "ResourceManagement/Scene.h"
#include "texture/TextureLoader.h"
#include "Components/MaterialComponent.h"
#include "Components/TransformComponent.h"

OpenGLRenderer::OpenGLRenderer()
    :  m_shaderManager(ShaderManager()),
        m_shadowMapBuffer(OpenGLShadowMapBuffer(4096, 4096)), m_frameBuffer(OpenGLFrameBuffer(2048, 2048)), m_quadBuffer(OpenGLQuadBuffer())
{

//TODO this needs to be loaded from a LUA config
    assert((defaultTexture = new Texture("Assets/default/default.jpg")));
    m_shaderManager.loadShader("lightingShader", "Vert.glsl", "Frag.glsl");
    m_shaderManager.loadShader("shadowShader", "shadow_vertex.glsl", "shadow_fragment.glsl");
    m_shaderManager.loadShader("framebufferShader", "Frame_Vert.glsl", "Frame_Frag.glsl");

}

OpenGLRenderer::~OpenGLRenderer()
{
    delete defaultTexture;
}

void OpenGLRenderer::Clear() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*
 * Renders the scene data to an image buffer, and returns the buffer
 */
unsigned int OpenGLRenderer::Render(Scene& scene, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& viewPos)
{
    const auto& lightingShader = m_shaderManager.getShader("lightingShader");
    const auto& framebufferShader = m_shaderManager.getShader("framebufferShader");
    
    ShadowPass(scene, m_shaderManager, m_shadowMapBuffer); // 4
    m_frameBuffer.bind();
    
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
    glEnable(GL_DEPTH_TEST);

    glm::mat4 lightSpaceMatrix = OpenGLShadowMapBuffer::CalculateLightSpaceMatrix(scene.getDirectionalLight().getDirection());
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
            } else {
                defaultTexture->bind(0);
                shader->SetUniform1i("albedoMap", 0);
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


void OpenGLRenderer::ShadowPass(Scene& scene, ShaderManager& shaderManager, IDataBuffer& shadowMap)
{
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glCullFace(GL_FRONT); // Avoid shadow acne

    shadowMap.bind();
    glClear(GL_DEPTH_BUFFER_BIT);


   
    // Iterate over entities with Mesh and Transform components
    auto view = scene.getEntityManager().view<RenderableComponent, TransformComponent>();
    for (auto entity : view) {
        auto& mesh = scene.getEntityManager().get<RenderableComponent>(entity);
        auto& transform = scene.getEntityManager().get<TransformComponent>(entity);
        auto& material = scene.getEntityManager().get<MaterialComponent>(entity);

        auto shadowShader = shaderManager.getShader("shadowShader"); // for now we'll hard code this, remove it from main.
        if (shadowShader->getID() != m_currentShaderID)
        {
            shadowShader->bind();
            m_currentShaderID = shadowShader->getID();

        }
        shadowShader->SetUniformMat4("lightSpaceMatrix", OpenGLShadowMapBuffer::CalculateLightSpaceMatrix(scene.getDirectionalLight().getDirection()));

        // check for decals and enable transparency CURRENTLY JUST NOT RENDERING DECAL MESHES
        if (!material.isDecal)
        {
            glm::mat4 modelMatrix = transform.getModelMatrix();
            shadowShader->SetUniformMat4("model", modelMatrix);

            mesh.meshBuffer->bind();
            mesh.meshBuffer->draw();
            glBindVertexArray(0);
        }
    }
    shadowMap.unbind();
    glCullFace(GL_BACK); // Reset to default cull face
    glDepthMask(GL_TRUE);
}
