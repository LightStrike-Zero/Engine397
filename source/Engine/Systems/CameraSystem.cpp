
#include <iostream>
#include "CameraSystem.h"

#include "EventSystem.h"

void CameraSystem::update(entt::registry& registry, float deltaTime, bool& showExitScreen)
{
    auto view = registry.view<CameraComponent, TransformComponent>();
        
    for (auto entity : view) {
        auto& camera = view.get<CameraComponent>(entity);
        auto& transform = view.get<TransformComponent>(entity);
            
        if (isActiveCamera(entity, registry)) {
            handleCameraInput(transform, camera, deltaTime, showExitScreen);
        }
    }
}

std::tuple<glm::mat4, glm::mat4, glm::vec3> CameraSystem::getActiveCameraMatrices(entt::registry& registry) {
    auto activeCameraEntity = getActiveCameraEntity(registry);
        
    if (activeCameraEntity == entt::null) {
        return {
            glm::mat4(1.0f),
            glm::perspective(glm::radians(45.0f), m_aspectRatio, 0.1f, 100.0f),
            glm::vec3(0.0f, 0.0f, 0.0f)
        };
    }
        
    auto& camera = registry.get<CameraComponent>(activeCameraEntity);
    auto& transform = registry.get<TransformComponent>(activeCameraEntity);
        
    return {
        camera.getViewMatrix(transform.position),
        camera.getProjectionMatrix(m_aspectRatio),
        transform.position
    };
}


//void CameraSystem::handleCameraInput(TransformComponent& transform, CameraComponent& camera, float deltaTime)
void CameraSystem::handleCameraInput(TransformComponent& transform, CameraComponent& camera, float deltaTime, bool& showExitScreen)
{
    // ESC TO EXIT ONLY
//    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//        glfwSetWindowShouldClose(m_window, true);
//    }

    // X TO EXIT ONLY
//    if (glfwGetKey(m_window, GLFW_KEY_X) == GLFW_PRESS)
//    {
//        glfwSetWindowShouldClose(m_window, true);
//    }

    //BUKO X TO EXIT AND SHOW EXIT IMAGE
    static bool xWasDown = false;
    if (glfwGetKey(m_window, GLFW_KEY_X) == GLFW_PRESS)
    {
        if (!xWasDown) {
            showExitScreen = !showExitScreen; // Toggle the flag
            xWasDown = true;
        }
    } else {
        xWasDown = false;
    }

//    if (showExitScreen)
//        std::cout << "[DEBUG] Exit screen is visible\n";
//    else
//        std::cout << "[DEBUG] Exit screen is NOT visible\n";
    //BUKO


    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS) {
        handleKeyboardInput(transform, camera, deltaTime);
            
        handleMouseInput(camera);
    }
}

void CameraSystem::handleKeyboardInput(TransformComponent& transform, CameraComponent& camera, float deltaTime)
{
    float velocity = camera.movementSpeed * deltaTime;
        
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        transform.position += camera.front * velocity;
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        transform.position -= camera.front * velocity;
    
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        transform.position -= camera.right * velocity;
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        transform.position += camera.right * velocity;

    float rotationVelocity = camera.rotationSpeed * deltaTime;
    if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.yaw -= rotationVelocity;
    if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.yaw += rotationVelocity;
    if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.pitch += rotationVelocity;
    if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.pitch -= rotationVelocity;

    if (camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if (camera.pitch < -89.0f)
        camera.pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    front.y = sin(glm::radians(camera.pitch));
    front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.front = glm::normalize(front);

    camera.right = glm::normalize(glm::cross(camera.front, camera.worldUp));
    camera.up = glm::normalize(glm::cross(camera.right, camera.front));

    static bool keyWasPressed = false;
    if (glfwGetKey(m_window, GLFW_KEY_C) == GLFW_PRESS) {
        if (!keyWasPressed) {
            static bool lineMode = false;
            lineMode = !lineMode;
            
            DrawModeChangedEvent event(lineMode);
            EventSystem::getInstance().dispatchEvent(event);
            
            keyWasPressed = true;
        }
    } else {
        keyWasPressed = false;
    }
}

void CameraSystem::handleMouseInput(CameraComponent& camera)
{
    static bool firstMouse = true;
    static float lastX = 0.0f;
    static float lastY = 0.0f;
        
    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);

    if (firstMouse) {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    float xOffset = static_cast<float>(xpos) - lastX;
    float yOffset = lastY - static_cast<float>(ypos);

    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    xOffset *= camera.mouseSensitivity;
    yOffset *= camera.mouseSensitivity;
        
    camera.yaw += xOffset;
    camera.pitch += yOffset;
        
    if (camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if (camera.pitch < -89.0f)
        camera.pitch = -89.0f;
        
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