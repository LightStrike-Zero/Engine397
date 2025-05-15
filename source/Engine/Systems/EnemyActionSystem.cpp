//
// Created by Hugo on 15/05/2025.
//

#include "EnemyActionSystem.h"
#include "Components/EnemyComponent.h"
#include "Components/PlayerControllerComponent.h"



void EnemyActionSystem::initialise(EnttFacade& entt){
    //TODO:read parameters from lua
    //TODO:build a listener for inter-NPC communication
    EventSystem::getInstance().addListener(EventType::EnemySpottedPlayerEvent,[this, &entt](const Event& event) {
            handlePlayerSpottedAlarm(dynamic_cast<const EnemySpottedPlayerEvent&>(event),entt);
        }
    );
}

void EnemyActionSystem::update(float deltaTime, EnttFacade& entt){
    //get the player transform
    auto playerView = entt.view<TransformComponent, PlayerControllerComponent>();
    auto& playerTransform = playerView.get<TransformComponent>(playerView.front());

    //scan and pull out all enemies
    auto view = entt.view<TransformComponent,AIScriptComponent,EnemyComponent>();
    //TODO: add animation component
    for (auto entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        auto& aiScript = view.get<AIScriptComponent>(entity);
        updateOneEnemy(transform,playerTransform, aiScript, deltaTime);
    }
}

void EnemyActionSystem::updateOneEnemy(TransformComponent& transform, TransformComponent& playerTransform, AIScriptComponent& aiScript, float deltaTime){
    switch (aiScript.currentState) {
        case AIScriptComponent::patrol:
            handlePatrolLogic(transform, playerTransform, aiScript, deltaTime);
            break;
        case AIScriptComponent::attack:
            handleAttackLogic(transform, playerTransform, aiScript, deltaTime);
            break;
        case AIScriptComponent::flee:
            handleFleeLogic(transform, playerTransform, aiScript, deltaTime);
            break;

        default: ;
    }
}

void EnemyActionSystem::handlePatrolLogic(TransformComponent& transform, TransformComponent& playerTransform, AIScriptComponent& aiScript, float deltaTime) {
    //find the player by itself
    glm::vec3 displacement = playerTransform.position - transform.position;
    float distance = glm::length(displacement);
    bool playerFound = distance < 50.0f; //replace 5,0f with a parameter from lua
    if (playerFound) {
        EnemySpottedPlayerEvent event(transform.position);//the spotter announces attack by notifying other enemies of its own position
        EventSystem::getInstance().dispatchEvent(event);

        aiScript.currentState = AIScriptComponent::attack;//transformer, attack mode
    }
    else if (true){}

    else { //walk to the next waypoint
        glm::vec3 destination(1.0f, 0.0f, 0.0f); // Example destination
        move(transform, destination, deltaTime);
        // TODO:Check if the enemy has reached the destination
    }


}
void EnemyActionSystem::handleAttackLogic(TransformComponent& transform, TransformComponent& playerTransform, AIScriptComponent& aiScript, float deltaTime) {
    //for now, just move towards the player, like zombies
    move(transform, playerTransform.position, deltaTime);
}
void EnemyActionSystem::handleFleeLogic(TransformComponent& transform, TransformComponent& playerTransform, AIScriptComponent& aiScript, float deltaTime){}

void EnemyActionSystem::move(TransformComponent& transform, glm::vec3 destination, float deltaTime) {
    glm::vec3 velocity = glm::normalize(destination - transform.position) * deltaTime; //could add a speed factor by calling m_parameters from lua
    transform.position += velocity;
    //TODO: add logic that resolves height according to the terrain
}

void EnemyActionSystem::handlePlayerSpottedAlarm(const EnemySpottedPlayerEvent& event, EnttFacade& entt) {
    //get the player transform
    auto playerView = entt.view<TransformComponent, PlayerControllerComponent>();
    auto& playerTransform = playerView.get<TransformComponent>(playerView.front());

    //scan and pull out all enemies
    auto view = entt.view<TransformComponent,EnemyComponent,AIScriptComponent>();
    for (auto entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        auto& aiScript = view.get<AIScriptComponent>(entity);
        if (glm::length(transform.position - playerTransform.position) < 50.0f) { //TODO: replace 5.0f with a parameter from lua
            aiScript.currentState = AIScriptComponent::attack;
        }
    }
}