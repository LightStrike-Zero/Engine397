
#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H
#include <entt/entt.hpp>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "Components/CameraComponent.h"
#include "Components/TransformComponent.h"

class CameraSystem {
public:
    CameraSystem(GLFWwindow* window, float aspectRatio)
        : m_window(window), m_aspectRatio(aspectRatio) {}
    
    //void update(entt::registry& registry, float deltaTime);
    void update(entt::registry& registry, float deltaTime, bool& showExitScreen, bool& showHelpScreen);
    
    std::tuple<glm::mat4, glm::mat4, glm::vec3> getActiveCameraMatrices(entt::registry& registry);
    
private:
    GLFWwindow* m_window;
    float m_aspectRatio;
    
    //void handleCameraInput(TransformComponent& transform, CameraComponent& camera, float deltaTime);
    void CameraSystem::handleCameraInput(TransformComponent& transform, CameraComponent& camera, float deltaTime, bool& showExitScreen, bool& showHelpScreen);

    void handleKeyboardInput(TransformComponent& transform, CameraComponent& camera, float deltaTime);
    
    void handleMouseInput(CameraComponent& camera);
    
    void updateCameraVectors(CameraComponent& camera);
    
    entt::entity getActiveCameraEntity(entt::registry& registry);
    
    bool isActiveCamera(entt::entity entity, entt::registry& registry);
};

#endif //CAMERASYSTEM_H
