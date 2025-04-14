/**
 * @file CameraSystem.h
 * @brief Handles camera input and updates camera rendering.
 *
 * This class provides functionality for camera control using keyboard and mouse,
 * and computes the camera's view and projection matrices based on its current state.
 * @author Probably Shaun
 * @date March 2025
 */

#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H
#include <entt/entt.hpp>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "Components/CameraComponent.h"
#include "Components/TransformComponent.h"
#include "ResourceManagement/EnttFacade.h"

/**
 * @class CameraSystem
 * @brief Manages camera input, updates, and matrix generation.
 */
class CameraSystem {
public:
    /**
     * @brief Constructs a CameraSystem.
     * @param window Pointer to the GLFW window.
     * @param aspectRatio The aspect ratio of the rendering viewport.
     */
    CameraSystem(GLFWwindow* window, float aspectRatio)
        : m_window(window), m_aspectRatio(aspectRatio) {}

    /**
    * @brief Updates the camera system per frame.
    * @param ecs Reference to the entity-component system facade.
    * @param deltaTime Time since last update.
    * @param showExitScreen Flag for toggling exit splash screen.
    * @param showHelpScreen Flag for toggling help overlay.
    */
    void update(EnttFacade& ecs, float deltaTime, bool& showExitScreen, bool& showHelpScreen);

    /**
     * @brief Returns the active camera's view, projection matrices, and position.
     * @param ecs Reference to the entity-component system.
     * @return Tuple containing view matrix, projection matrix, and camera position.
     */
    std::tuple<glm::mat4, glm::mat4, glm::vec3> getActiveCameraMatrices(EnttFacade& ecs);
    
private:
    GLFWwindow* m_window;
    float m_aspectRatio;
    
    //void handleCameraInput(TransformComponent& transform, CameraComponent& camera, float deltaTime);
    void handleCameraInput(TransformComponent& transform, CameraComponent& camera, float deltaTime, bool& showExitScreen, bool& showHelpScreen);

    void handleKeyboardInput(TransformComponent& transform, CameraComponent& camera, float deltaTime);
    
    void handleMouseInput(CameraComponent& camera);

    void updateCameraVectors(CameraComponent& camera);
    
    EnttFacade::Entity getActiveCameraEntity(EnttFacade::Registry& registry); //Hugo: from what i understand, this only returns the first camera entity in the registry, even if there are many
    
    bool isActiveCamera(EnttFacade::Entity entity, EnttFacade::Registry& registry);


};

#endif //CAMERASYSTEM_H
