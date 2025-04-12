////
//// Created by Hugo on 4/12/2025.
////
//
//#include "DebugColliderRenderer.h"
//#include <glm/gtc/matrix_transform.hpp>
//#include <GL/glew.h>
//#include "Rendering/RenderableComponent.h"
//#include "OpenGL/OpenGLMeshBuffer.h"
//
//DebugColliderRenderer::DebugColliderRenderer(EnttFacade* ecs)
//    : m_ents(ecs)
//{
//    // Load or generate a wireframe cube mesh
//    // Assume unit cube centered at origin (from -0.5 to +0.5)
//    m_wireCube = generateWireframeCubeMesh(); // You implement this
//}
//
//void DebugColliderRenderer::render() {
//    if (!enabled) return;
//
//    auto view = m_ents->view<TransformComponent, BoxColliderComponent>();
//
//    // Set up wireframe mode and color
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    glDisable(GL_DEPTH_TEST); // optional: always visible
//
//    for (auto entity : view) {
//        auto& transform = view.get<TransformComponent>(entity);
//        auto& collider = view.get<BoxColliderComponent>(entity);
//
//        glm::vec3 center = transform.position + collider.offset;
//        glm::vec3 scale = collider.halfExtents * 2.0f;
//
//        glm::mat4 model = glm::translate(glm::mat4(1.0f), center);
//        model = glm::scale(model, scale);
//
//        // Upload model matrix to your shader
//        yourDebugShader.setMat4("u_Model", model);
//        yourDebugShader.setVec3("u_Color", glm::vec3(1, 0, 0)); // red box
//
//        // Render the cube
//        m_wireCube.meshBuffer->draw();
//    }
//
//    // Reset state
//    glEnable(GL_DEPTH_TEST);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//}
//
//RenderableComponent DebugColliderRenderer::generateWireframeCubeMesh() {
//    RenderableComponent wireCube;
//
//    // Define cube corners (8 vertices)
//    std::vector<glm::vec3> positions = {
//        {-0.5f, -0.5f, -0.5f},
//        { 0.5f, -0.5f, -0.5f},
//        { 0.5f,  0.5f, -0.5f},
//        {-0.5f,  0.5f, -0.5f},
//        {-0.5f, -0.5f,  0.5f},
//        { 0.5f, -0.5f,  0.5f},
//        { 0.5f,  0.5f,  0.5f},
//        {-0.5f,  0.5f,  0.5f}
//    };
//
//    // Edges: 12 lines (24 indices)
//    std::vector<uint32_t> indices = {
//        0,1, 1,2, 2,3, 3,0, // bottom face
//        4,5, 5,6, 6,7, 7,4, // top face
//        0,4, 1,5, 2,6, 3,7  // vertical edges
//    };
//
//    // Convert positions to Vertex struct if needed
//    wireCube.vertices.clear();
//    for (const auto& pos : positions) {
//        wireCube.vertices.push_back({ pos }); // Assume Vertex{glm::vec3 pos;}
//    }
//
//    wireCube.indices = indices;
//
//    wireCube.meshBuffer = std::make_shared<OpenGLMeshBuffer>(
//        indices.size(), positions.size(),
//        indices.data(), wireCube.vertices.data(),
//        GL_LINES // draw lines!
//    );
//
//    return wireCube;
//}