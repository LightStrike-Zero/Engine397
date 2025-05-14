#include "Player.h"

#include <iostream>

#include "Components/CameraComponent.h"
#include "Physics_DEPRECIATED//TankCollision.h"


void Player::update(float deltaTime,ScriptManager* scriptManager, InputManager& inputManager) {
    //get player & camera entity's components
    auto playerView = m_entt->view<PlayerControllerComponent, TransformComponent, BoxColliderComponent>();
    auto playerEntity = *playerView.begin();
    auto& playerTransform = playerView.get<TransformComponent>(playerEntity);
    auto& playerCollider = m_entt->get<BoxColliderComponent>(playerEntity);
    TransformComponent originalPlayerTransform = playerTransform; // save to revert on collision

    auto cameraView = m_entt->view<TransformComponent, CameraComponent>();
    auto cameraEntity = *cameraView.begin();
    TransformComponent& cameraTransform = cameraView.get<TransformComponent>(cameraEntity);    //get camera transform
    CameraComponent& camera = cameraView.get<CameraComponent>(cameraEntity);   //get camera component
    TransformComponent originalCameraTransform = cameraTransform;
    CameraComponent originalCamera = camera;

    //WASD movement
    handleMovementInput(playerTransform, playerCollider, scriptManager, inputManager, deltaTime);

    //resetting the player transform to original position when pressing R or when moving over the edge
    handlePlayerInput(playerTransform, scriptManager);


    // Check for collisions
    auto collidableView = m_entt->view<TransformComponent, BoxColliderComponent>(exclude<PlayerControllerComponent>);
    for (auto other : collidableView) {
        if (other == playerEntity) continue; // skip self

        auto& otherTransform = collidableView.get<TransformComponent>(other);
        auto& otherCollider = collidableView.get<BoxColliderComponent>(other);

        if (checkBoxtoBoxCollision(playerCollider, playerTransform, otherCollider, otherTransform)) {
            playerTransform = originalPlayerTransform; // cancel movement
            cameraTransform = originalCameraTransform;
            camera = originalCamera; // cancel camera movement
            break;
        }
    }



    // for (auto entity : cannonView) {
    //     auto& cannonTransform = cannonView.get<TransformComponent>(entity);
    //     auto& sphereCollider = cannonView.get<SphereColliderComponent>(entity);
    //     shootCannon(cannonTransform,sphereCollider, deltaTime);
    // }
}
void Player::handleMovementInput(TransformComponent& playerTankTransform, BoxColliderComponent& collider, ScriptManager* scriptManager, InputManager& inputManager, float deltaTime) const {
    float rotationVelocity = rotationSpeed * deltaTime;
    glm::vec3 forward;
    forward.x = -sin(glm::radians(playerTankTransform.rotation.y));
    forward.y = 0.0f;
    forward.z = -cos(glm::radians(playerTankTransform.rotation.y));
    const glm::vec3 velocity = forward * deltaTime * movementSpeed;

    //adjust camera so that it rotates with the tank
    auto cameraView = m_entt->view<TransformComponent, CameraComponent>();
    auto cameraEntity = *cameraView.begin();
    TransformComponent& cameraTransform = cameraView.get<TransformComponent>(cameraEntity);    //get camera transform
    CameraComponent& camera = cameraView.get<CameraComponent>(cameraEntity);   //get camera component

    glm::vec3 cameraOffset = scriptManager->getVec3("cameraOffset");
    cameraTransform.position.y = playerTankTransform.position.y + cameraOffset.y;//set camera height to tank height, which is modified by terrain
    // Step 1: Get vector from tank to camera
    glm::vec3 radiusVec = cameraTransform.position - playerTankTransform.position;

    // Step 2: Flatten to XZ plane (no vertical movement)
    radiusVec.y = 0.0f;

    // Step 3: Normalize and get distance
    float radius = glm::length(radiusVec);

    glm::vec3 radiusDir = glm::normalize(radiusVec);

    // Step 4: Get perpendicular direction (90° rotation in XZ plane)
    glm::vec3 tangentDir = glm::vec3(radiusDir.z, 0.0f, -radiusDir.x);  // left turn (KEY_A)

    // Step 5: Compute arc velocity
    float arcSpeed = radius * glm::radians(rotationVelocity);
    glm::vec3 cameraVelocity = tangentDir * arcSpeed;


    if (inputManager.isKeyDown(GLFW_KEY_W))
    {
        playerTankTransform.position -= velocity;
        cameraTransform.position -= velocity;
    }
    if (inputManager.isKeyDown(GLFW_KEY_S))
    {
        playerTankTransform.position += velocity;
        cameraTransform.position += velocity;
    }
    if (inputManager.isKeyDown(GLFW_KEY_A))
    {
        playerTankTransform.rotation.y += rotationVelocity;
        camera.yaw -= rotationVelocity;
        cameraTransform.position += cameraVelocity;

        //---- end ----
    }
    if (inputManager.isKeyDown(GLFW_KEY_D))
    {
        playerTankTransform.rotation.y -= rotationVelocity;
        camera.yaw += rotationVelocity;
        cameraTransform.position -= cameraVelocity;

        //same as above

        //end


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
    //get camera
    auto cameraView = m_entt->view<TransformComponent, CameraComponent>();
    auto cameraEntity = *cameraView.begin();
    TransformComponent& cameraTransform = cameraView.get<TransformComponent>(cameraEntity);    //get camera transform
    CameraComponent& camera = cameraView.get<CameraComponent>(cameraEntity);   //get camera component
    glm::vec3 cameraOffset = scriptManager->getVec3("cameraOffset");

    //hugo playertank reset
    static bool reset = false;
    glm::vec3 resetPosition = scriptManager->getVec3("playerResetPosition");
    glm::vec3 resetRotation = scriptManager->getVec3("playerResetRotation");
    // glm::vec3 resetRotation = {0.f, 180.f, 0.f};

    if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS) {
        if (!reset) {
            playerTransform.position = resetPosition;
            playerTransform.rotation = resetRotation;
            m_entt->replaceComponent(cameraEntity, CameraComponent());
            cameraTransform.position = playerTransform.position + cameraOffset;

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
        playerTransform.rotation = resetRotation;
        m_entt->replaceComponent(cameraEntity, CameraComponent());
        cameraTransform.position = playerTransform.position + cameraOffset;
    }
}

