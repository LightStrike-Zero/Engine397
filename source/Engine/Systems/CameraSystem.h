
#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H
#include <entt/entt.hpp>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "Components/CameraComponent.h"
#include "Components/TransformComponent.h"
#include "ResourceManagement/EnttFacade.h"

class CameraSystem {
public:
    CameraSystem(GLFWwindow* window, float aspectRatio)
        : m_window(window), m_aspectRatio(aspectRatio) {}
    
    void update(EnttFacade& ecs, float deltaTime);
    
    std::tuple<glm::mat4, glm::mat4, glm::vec3> getActiveCameraMatrices(EnttFacade& ecs);
    
private:
    GLFWwindow* m_window;
    float m_aspectRatio;
    
    void handleCameraInput(TransformComponent& transform, CameraComponent& camera, float deltaTime);
    
    void handleKeyboardInput(TransformComponent& transform, CameraComponent& camera, float deltaTime);
    
    void handleMouseInput(CameraComponent& camera);
    
    void updateCameraVectors(CameraComponent& camera);
    
    EnttFacade::Entity getActiveCameraEntity(EnttFacade::Registry& registry);
    
    bool isActiveCamera(EnttFacade::Entity entity, EnttFacade::Registry& registry);
};

#endif //CAMERASYSTEM_H
