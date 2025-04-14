#include "Player.h"

#include <iostream>
#include "Physics_DEPRECIATED//TankCollision.h"


void Player::update(float deltaTime,ScriptManager* scriptManager) {
    auto playerView = m_entt->view<PlayerControllerComponent, TransformComponent, BoxColliderComponent>();

    // scriptManager->runScript("GameScripts/GameConfig.lua");
    //get components needed for player movement
    for (auto entity : playerView) {
        auto& playerTransform = playerView.get<TransformComponent>(entity);
        // auto& boxCollider = playerView.get<BoxColliderComponent>(entity);
        auto& playerCollider = m_entt->get<BoxColliderComponent>(entity);
        TransformComponent originalTransform = playerTransform; // save to revert on collision
        handleMovementInput(playerTransform, playerCollider, deltaTime);
        //resetting the player transform to original position when pressing R or when moving over the edge

        handlePlayerInput(playerTransform, scriptManager);


        // Check for collisions
        auto collidableView = m_entt->view<TransformComponent, BoxColliderComponent>(exclude<PlayerControllerComponent>);
        for (auto other : collidableView) {
            if (other == entity) continue; // skip self

            auto& otherTransform = collidableView.get<TransformComponent>(other);
            auto& otherCollider = collidableView.get<BoxColliderComponent>(other);

            if (checkBoxtoBoxCollision(playerCollider, playerTransform, otherCollider, otherTransform)) {
            // if (checkCollision(playerCollider, playerTransform, otherCollider, otherTransform)) {
                playerTransform = originalTransform; // cancel movement
                break;
            }
        }

    }
    // for (auto entity : cannonView) {
    //     auto& cannonTransform = cannonView.get<TransformComponent>(entity);
    //     auto& sphereCollider = cannonView.get<SphereColliderComponent>(entity);
    //     shootCannon(cannonTransform,sphereCollider, deltaTime);
    // }
}
void Player::handleMovementInput(TransformComponent& transform, BoxColliderComponent& collider, float deltaTime) const {
    float rotationVelocity = rotationSpeed * deltaTime;
    glm::vec3 forward;
    forward.x = -sin(glm::radians(transform.rotation.y));
    forward.y = 0.0f;
    forward.z = -cos(glm::radians(transform.rotation.y));
    const glm::vec3 velocity = forward * deltaTime * movementSpeed;



    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
    {
        transform.position -= velocity;
    }
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
    {
        transform.position += velocity;
    }
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
    {
        transform.rotation.y += rotationVelocity;
    }
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
    {
        transform.rotation.y -= rotationVelocity;
    }
}
void Player::shootCannon(TransformComponent cannonTransform, SphereColliderComponent sphereCollider, float deltaTime) {

}

bool Player::checkCollision(const BoxColliderComponent& a, const TransformComponent& aTransform,
                            const BoxColliderComponent& b, const TransformComponent& bTransform) const {
    glm::vec3 aMin = aTransform.position + a.offset - a.halfExtents;
    glm::vec3 aMax = aTransform.position + a.offset + a.halfExtents;

    glm::vec3 bMin = bTransform.position + b.offset - b.halfExtents;
    glm::vec3 bMax = bTransform.position + b.offset + b.halfExtents;

    bool flag =  (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
           (aMin.y <= bMax.y && aMax.y >= bMin.y) &&
           (aMin.z <= bMax.z && aMax.z >= bMin.z);
    std::cout << flag << std::endl;
    return flag;
}

void Player::handlePlayerInput(TransformComponent& playerTransform, ScriptManager* scriptManager) {
    //hugo playertank reset
    static bool reset = false;
    glm::vec3 resetPosition = scriptManager->getVec3FromLua("playerResetPosition");
    if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS) {
        if (!reset) {
            playerTransform.position = resetPosition;
            reset = true;
        }
    } else {
        reset = false;
    }
    //reset when going over edges
    int rowSize = scriptManager->getTerrainRows();
    int colSize = scriptManager->getTerrainCols();
    if (playerTransform.position.x > rowSize / 2 || playerTransform.position.x < -rowSize / 2 ||
        playerTransform.position.z > colSize / 2 || playerTransform.position.z < -colSize / 2) {
        playerTransform.position = resetPosition;
    }
}

