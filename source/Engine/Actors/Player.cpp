#include "Player.h"

void Player::update(float deltaTime) {
    // auto playerView = m_entt->view<PlayerControllerComponent, TransformComponent, BoxColliderComponent>();
    auto playerView = m_entt->view<PlayerControllerComponent, TransformComponent>();
    auto cannonView = m_entt->view<SphereColliderComponent, TransformComponent>();

    for (auto entity : playerView) {
        auto& playerTransform = playerView.get<TransformComponent>(entity);
        // auto& boxCollider = playerView.get<BoxColliderComponent>(entity);
        auto *boxCollider = new BoxColliderComponent{glm::vec3{1.f}, glm::vec3{1.f}};
        handleMovementInput(playerTransform, *boxCollider, deltaTime);
    }
    for (auto entity : cannonView) {
        auto& cannonTransform = cannonView.get<TransformComponent>(entity);
        auto& sphereCollider = cannonView.get<SphereColliderComponent>(entity);
        shootCannon(cannonTransform,sphereCollider, deltaTime);
    }
}
void Player::handleMovementInput(TransformComponent& transform, BoxColliderComponent& collider, float deltaTime) const {
    float rotationVelocity = rotationSpeed * deltaTime;
    glm::vec3 forward;
    forward.x = -sin(glm::radians(transform.rotation.y));
    forward.y = 0.0f;
    forward.z = -cos(glm::radians(transform.rotation.y));
    glm::vec3 velocity = forward * deltaTime * movementSpeed;



    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        transform.position -= velocity;
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        transform.position += velocity;
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        transform.rotation.y += rotationVelocity;
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        transform.rotation.y -= rotationVelocity;
}
void Player::shootCannon(TransformComponent cannonTransform, SphereColliderComponent sphereCollider, float deltaTime) {

}

