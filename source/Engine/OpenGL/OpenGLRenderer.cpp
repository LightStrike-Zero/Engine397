//
// Created by Shaun on 15/03/2025.


#include "OpenGLRenderer.h"

#include <iostream>
#include <Components/RenderableComponent.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "OpenGLShadowMapBuffer.h"

#include "Components/MaterialComponent.h"
#include "Components/SkyboxComponent.h"
#include "Components/TransformComponent.h"
#include "Components/CollisionComponents/BoxColliderComponent.h"
#include "ResourceManagement/Scene.h"
#include "texture/TextureLoader.h"

OpenGLRenderer::OpenGLRenderer()
    : m_shaderManager(ShaderManager()),
      m_shadowMapBuffer(OpenGLShadowMapBuffer(4096, 4096)), m_frameBuffer(OpenGLFrameBuffer(2048, 2048)),
      m_quadBuffer(OpenGLQuadBuffer())
{
    m_shaderManager.loadShader("lightingShader", "Vert.glsl", "Frag.glsl");
    m_shaderManager.loadShader("skyboxShader", "skybox_vert.glsl", "skybox_frag.glsl");
    m_shaderManager.loadShader("shadowShader", "shadow_vertex.glsl", "shadow_fragment.glsl");
    m_shaderManager.loadShader("framebufferShader", "Frame_Vert.glsl", "Frame_Frag.glsl");
    m_shaderManager.loadShader("waterShader", "WaterVert.glsl", "WaterFrag.glsl");
    m_shaderManager.loadShader("debugShader", "debug_vs.glsl", "debug_fs.glsl");

    //TODO this stuff is for the debug wireframe cube - will move it somewhere else later
    std::vector<Vertex> vertices = {
        // Front face
        {{-0.5f, -0.5f, 0.5f}}, {{0.5f, -0.5f, 0.5f}},
        {{0.5f, 0.5f, 0.5f}}, {{-0.5f, 0.5f, 0.5f}},
        // Back face
        {{-0.5f, -0.5f, -0.5f}}, {{0.5f, -0.5f, -0.5f}},
        {{0.5f, 0.5f, -0.5f}}, {{-0.5f, 0.5f, -0.5f}}
    };

    // Indices for wireframe rendering
    std::vector<unsigned int> indices = {
        // Front face
        0, 1, 1, 2, 2, 3, 3, 0,
        // Back face
        4, 5, 5, 6, 6, 7, 7, 4,
        // Connecting edges
        0, 4, 1, 5, 2, 6, 3, 7
    };

    m_debugWireframeCube = new OpenGLMeshBuffer(indices.size(), vertices.size(), indices.data(), vertices.data());
    m_debugWireframeCube->m_lineMode = true;

    m_showColliders = true;
    
}

OpenGLRenderer::~OpenGLRenderer()
{
    if (m_debugWireframeCube)
    {
        delete m_debugWireframeCube;
        m_debugWireframeCube = nullptr;
    }
    
}

void OpenGLRenderer::DebugCollidersPass(Scene& scene, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
    if (!m_showColliders) return;
    const auto debugShader = m_shaderManager.getShader("debugShader");
   
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    debugShader->bind();
    debugShader->SetUniformMat4("u_ViewMatrix", viewMatrix);
    debugShader->SetUniformMat4("u_ProjectionMatrix", projectionMatrix);

    const auto view = scene.getEntityManager().view<TransformComponent, BoxColliderComponent>();

    for (const auto entity : view)
    {
        auto& transform = view.get<TransformComponent>(entity);
        auto& collider = view.get<BoxColliderComponent>(entity);

        auto model = glm::mat4(1.0f);
        model = glm::translate(model, transform.position + collider.offset);

        // Apply rotation
        if (transform.rotation != glm::vec3(0.0f))
        {
            model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));
            model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));
            model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));
        }

        // Scale by the half extents * 2 since our cube is from -0.5 to 0.5
        model = glm::scale(model, collider.halfExtents * 2.0f);

        debugShader->SetUniformMat4("u_ModelMatrix", model);
        debugShader->SetUniform3f("u_Color", glm::vec3(1.0f, 0.64f, 0.0f)); 

        m_debugWireframeCube->bind();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_debugWireframeCube->draw();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glDepthMask(GL_TRUE);
    debugShader->unbind();
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
    glm::mat4 lightSpaceMatrix = OpenGLShadowMapBuffer::CalculateLightSpaceMatrix(
        scene.getDirectionalLight().getDirection(), playerPos);
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
    WaterPass(scene, viewMatrix, projectionMatrix, viewPos);


    DebugCollidersPass(scene, viewMatrix, projectionMatrix);
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
    for (auto entity : view)
    {
        auto& mesh = view.get<RenderableComponent>(entity);
        auto& transform = view.get<TransformComponent>(entity);
        auto& material = view.get<MaterialComponent>(entity);

        auto shader = shaderManager.getShader(material.shaderID);
        if (material.shaderID == "waterShader") //TODO messy :( need water component so we can separate them out
            continue;
        // essentially we just want to check if the currently bound shader is the same as the shader we want to use
        if (shader->getID() != m_currentShaderID)
        {
            shader->bind();
            m_currentShaderID = shader->getID();
        }

        glm::mat4 modelMatrix = transform.getModelMatrix();
        shader->SetUniformMat4("model", modelMatrix);


        // Bind the diffuse texture
        if (material.baseColorTextureID != 0)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, material.baseColorTextureID);
            shader->SetUniform1i("albedoMap", 0);
            shader->SetUniform1i("hasAlbedoMap", 1);
        }
        else
        {
            shader->SetUniform1i("hasAlbedoMap", 0);
            shader->SetUniform3f("fallbackColor", {1.0f, 0.0f, 0.5f});
        }
        // Bind the detail texture
        if (material.detailTextureID != 0)
        {
            // std::cout << "Detail texture ID: " << material.detailTextureID << std::endl;
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, material.detailTextureID);
            shader->SetUniform1i("detailMap", 1);
            shader->SetUniform1i("hasDetailMap", 1);
            shader->SetUniform1f("detailScale", 40.0); // How much to scale the detail texture UVs
            shader->SetUniform1f("detailStrength", 0.5); // Blend factor
        }
        else
        {
            shader->SetUniform1i("hasDetailMap", 0);
        }

        mesh.meshBuffer->bind();
        mesh.meshBuffer->draw();
        glBindVertexArray(0);
        // don't unbind the shader, since we are LIKELY to use it again
    }
}


void OpenGLRenderer::SkyboxPass(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, Scene& scene,
                                ShaderManager& shaderManager)
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
    for (auto entity : skyboxView)
    {
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
    for (auto entity : view)
    {
        auto& mesh = scene.getEntityManager().get<RenderableComponent>(entity);
        auto& transform = scene.getEntityManager().get<TransformComponent>(entity);

        auto shadowShader = shaderManager.getShader("shadowShader"); // for now we'll hard code this
        if (shadowShader->getID() != m_currentShaderID)
        {
            shadowShader->bind();
            m_currentShaderID = shadowShader->getID();
        }
        auto playerView = scene.getEntityManager().view<TransformComponent, PlayerControllerComponent>();
        for (auto entity : playerView)
        {
            auto& playerTransform = playerView.get<TransformComponent>(entity);
            glm::vec3 playerPos = playerTransform.position;
            shadowShader->SetUniformMat4("lightSpaceMatrix",
                                         OpenGLShadowMapBuffer::CalculateLightSpaceMatrix(
                                             scene.getDirectionalLight().getDirection(), playerPos));
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

void OpenGLRenderer::WaterPass(Scene& scene, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& viewPos)
{
    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_CULL_FACE); // water visible from above and below


    auto waterShader = m_shaderManager.getShader("waterShader");
    waterShader->bind();
    m_currentShaderID = waterShader->getID();
    auto currentTime = static_cast<float>(glfwGetTime()); //TODO this is purely as a test - will not stay like this at all

    waterShader->SetUniform1f("time", currentTime);

    waterShader->SetUniformMat4("view", viewMatrix);
    waterShader->SetUniformMat4("projection", projectionMatrix);
    waterShader->SetUniform3f("viewPos", viewPos);

    waterShader->SetUniform3f("light.direction", scene.getDirectionalLight().getDirection());
    waterShader->SetUniform3f("light.ambient", scene.getDirectionalLight().getAmbient());
    waterShader->SetUniform3f("light.diffuse", scene.getDirectionalLight().getDiffuse());
    waterShader->SetUniform3f("light.specular", scene.getDirectionalLight().getSpecular());

    // glDepthFunc(GL_LEQUAL);

    glDepthFunc(GL_LESS); // default test
    glDepthMask(GL_FALSE); // don’t write depth, we only need the test
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto view = scene.getEntityManager().view<RenderableComponent, TransformComponent, MaterialComponent>();
    for (auto entity : view)
    {
        auto& material = view.get<MaterialComponent>(entity);
        if (material.shaderID != "waterShader") continue;

        auto& mesh = view.get<RenderableComponent>(entity);
        auto& transform = view.get<TransformComponent>(entity);

        waterShader->SetUniformMat4("model", transform.getModelMatrix());

        waterShader->SetUniform3f("waterColor", glm::vec3(0.0f, 0.4f, 0.8f)); // Ocean blue
        waterShader->SetUniform1f("waterAlpha", 0.8f);
        waterShader->SetUniform1f("reflectivity", 1.5f);
        waterShader->SetUniform1f("shineDamper", 32.0f);

        mesh.meshBuffer->bind();
        mesh.meshBuffer->draw();
        glBindVertexArray(0);
    }


    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE); // restore
    glEnable(GL_CULL_FACE); // restore
}
