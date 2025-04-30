
#include "CameraSystem.h"

#include <iostream>

#include "EventSystem.h"
#include "Components/PlayerControllerComponent.h"
#include "ResourceManagement/EnttFacade.h"

void CameraSystem::update(EnttFacade& ecs, float deltaTime, bool& showExitScreen, bool& showHelpScreen)
{
    auto view = ecs.view<CameraComponent, TransformComponent>();
        
    for (auto entity : view) {
        auto& camera = view.get<CameraComponent>(entity);
        auto& transform = view.get<TransformComponent>(entity);
            
        if (isActiveCamera(entity, ecs.getRegistry())) {
            handleCameraInput(transform, camera, deltaTime, showExitScreen, showHelpScreen);
        }
    }
    
}

std::tuple<glm::mat4, glm::mat4, glm::vec3> CameraSystem::getActiveCameraMatrices(EnttFacade& ecs) {
    auto activeCameraEntity = getActiveCameraEntity(ecs.getRegistry());
        
    if (activeCameraEntity == entt::null) {
        return {
            glm::mat4(1.0f),
            glm::perspective(glm::radians(45.0f), m_aspectRatio, 0.1f, 100.0f),
            glm::vec3(0.0f, 0.0f, 0.0f)
        };
    }
        
    auto& camera = ecs.getRegistry().get<CameraComponent>(activeCameraEntity);
    auto& transform = ecs.getRegistry().get<TransformComponent>(activeCameraEntity);
        
    return {
        camera.getViewMatrix(transform.position),
        camera.getProjectionMatrix(m_aspectRatio),
        transform.position
    };
}


//void CameraSystem::handleCameraInput(TransformComponent& transform, CameraComponent& camera, float deltaTime)
void CameraSystem::handleCameraInput(TransformComponent& transform,
                                     CameraComponent& camera,
                                     float deltaTime,
                                     bool& showExitScreen,
                                     bool& showHelpScreen)
{
    // --- exit toggle (X/Esc) ---
    static bool exitPressed = false;
    if (glfwGetKey(m_window, GLFW_KEY_X) == GLFW_PRESS ||
        glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        if (!exitPressed) {
            showExitScreen = !showExitScreen;
            exitPressed    = true;
        }
    } else {
        exitPressed = false;
    }

    // --- help toggle (M) ---
    static bool mWasDown = false;
    if (glfwGetKey(m_window, GLFW_KEY_M) == GLFW_PRESS && !mWasDown) {
        showHelpScreen = !showHelpScreen;
    }
    mWasDown = (glfwGetKey(m_window, GLFW_KEY_M) == GLFW_PRESS);

    // --- only fly/look while RMB is down ---
    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        // hide & capture cursor
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        handleMouseInput(camera);
        handleKeyboardInput(transform, camera, deltaTime);
    }
    else {
        // release & show cursor, reset first-mouse so we don't jump
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        m_firstMouse = true;
    }
}

void CameraSystem::handleKeyboardInput(TransformComponent& transform,
                                       CameraComponent& camera,
                                       float deltaTime)
{
    glm::vec3 dir(0.0f);
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        dir += camera.front;
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        dir -= camera.front;
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        dir -= camera.right;
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        dir += camera.right;

    if (glm::length(dir) > 0.0f) {
        dir = glm::normalize(dir);
        transform.position += dir * camera.movementSpeed * deltaTime;
    }
}

void CameraSystem::handleMouseInput(CameraComponent& camera)
{
    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);

    if (m_firstMouse) {
        m_lastX      = (float)xpos;
        m_lastY      = (float)ypos;
        m_firstMouse = false;
    }

    float xOffset = (float)xpos - m_lastX;
    float yOffset = m_lastY - (float)ypos;
    m_lastX = (float)xpos;
    m_lastY = (float)ypos;

    xOffset *= camera.mouseSensitivity;
    yOffset *= camera.mouseSensitivity;

    camera.yaw   += xOffset;
    camera.pitch += yOffset;
    camera.pitch  = glm::clamp(camera.pitch, -89.0f, 89.0f);

    updateCameraVectors(camera);
}

void CameraSystem::updateCameraVectors(CameraComponent& camera)
{
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    newFront.y = sin(glm::radians(camera.pitch));
    newFront.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
        
    camera.front = glm::normalize(newFront);
    camera.right = glm::normalize(glm::cross(camera.front, glm::vec3(0.0f, 1.0f, 0.0f)));
    camera.up = glm::normalize(glm::cross(camera.right, camera.front));
}

entt::entity CameraSystem::getActiveCameraEntity(entt::registry& registry) {
    auto view = registry.view<CameraComponent, TransformComponent>();
    for (auto entity : view) {
        return entity;
    }
        
    return entt::null;
}
bool CameraSystem::isActiveCamera(entt::entity entity, entt::registry& registry) {
    return entity == getActiveCameraEntity(registry);
}


