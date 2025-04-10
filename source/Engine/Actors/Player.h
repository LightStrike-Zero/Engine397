//
// Created by hugo on 8/04/2025.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <entt/entt.hpp>

#include "ResourceManagement/Scene.h"
#include "Components/TransformComponent.h"
#include "Components/CollisionComponents/BoxColliderComponent.h"
#include "ResourceManagement/EnttFacade.h"
#include "Window/glfwWindow.h"

class Player {
    // void loadPlayerModelToRegistry(Scene scene, const std::string& filepath);
public:
    Player() = default;
    void update(EnttFacade& ecs, float deltaTime);
    void handleMovementInput(TransformComponent& transform, BoxColliderComponent& collider, float deltaTime);
private:
    GLFWwindow* m_window;
};



// #endif //PLAYER_H
