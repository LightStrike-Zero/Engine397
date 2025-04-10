//
// Created by hugo on 8/04/2025.
//

#ifndef PLAYER_H
#define PLAYER_H

//#include <entt/entt.hpp>

#include "ResourceManagement/Scene.h"
#include "Components/TransformComponent.h"
#include "Components/PlayerControllerComponent.h"
#include "Components/CollisionComponents/SphereColliderComponent.h"
#include "Components/CollisionComponents/BoxColliderComponent.h"
#include "ResourceManagement/EnttFacade.h"
#include "Window/glfwWindow.h"

class Player {
    // void loadPlayerModelToRegistry(Scene scene, const std::string& filepath);
public:
    explicit Player(EnttFacade* ecs, GLFWwindow *window): m_entt(ecs), m_window(window) {}
    void update( float deltaTime);
    void handleMovementInput(TransformComponent& transform, BoxColliderComponent& collider, float deltaTime) const;
    void shootCannon(TransformComponent cannonTransform, SphereColliderComponent sphereCollider, float deltaTime);

private:
    EnttFacade* m_entt;
    GLFWwindow* m_window; //this actually calls glfw3.h instead our own facade, may need abstraction later
    float movementSpeed = 40.f;
    float rotationSpeed = 90.0f;
};



#endif //PLAYER_H
