//
// Created by hugo on 8/04/2025.
//
/// THIS IS NOT COMPLETE

/**
 * @file Player.h
 * @brief Defines the Player class used to represent the player.
 * @author Hugo
 * @date 2025-04-08
 */
#ifndef PLAYER_H
#define PLAYER_H

//#include <entt/entt.hpp>

#include <Scripting/ScriptManager.h>

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
    explicit Player(EnttFacade* ecs, GLFWwindow *window, float movementSpeed, float rotationSpeed): m_entt(ecs), m_window(window), movementSpeed(movementSpeed), rotationSpeed(rotationSpeed) {}
    void update( float deltaTime , ScriptManager* scriptManager);
    void handleMovementInput(TransformComponent& transform, BoxColliderComponent& collider, ScriptManager* scriptManager, float deltaTime) const;
    void shootCannon(TransformComponent cannonTransform, SphereColliderComponent sphereCollider, float deltaTime);



private:
    void handlePlayerInput(TransformComponent& playerTransform, ScriptManager* scriptManager);
    bool checkCollision(const BoxColliderComponent& a, const TransformComponent& aTransform,
                const BoxColliderComponent& b, const TransformComponent& bTransform) const;
    EnttFacade* m_entt;
    GLFWwindow* m_window; //this actually calls glfw3.h instead our own facade, may need abstraction later
    float movementSpeed;
    float rotationSpeed;
};



#endif //PLAYER_H

