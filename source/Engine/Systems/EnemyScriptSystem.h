//
// Created by Hugo on 15/05/2025.
//

#ifndef ENEMYACTIONSYSTEM_H
#define ENEMYACTIONSYSTEM_H
#include "EventSystem.h"
#include "Components/AIScriptComponent.h"
#include "Components/TransformComponent.h"
#include "ResourceManagement/EnttFacade.h"


class EnemyScriptSystem {
public:
    EnemyScriptSystem() = default;
    ~EnemyScriptSystem() = default;

    void initialise(EnttFacade& entt);
    void update(float deltaTime, EnttFacade& entt);
private:
    void updateOneEnemy(entt::entity entity, TransformComponent& transform, TransformComponent& playerTransform, AIScriptComponent& aiScript, float deltaTime);
    void handlePatrolLogic(TransformComponent& transform, TransformComponent& playerTransform, AIScriptComponent& aiScript, float deltaTime);
    void handleAttackLogic(TransformComponent& transform, TransformComponent& playerTransform, AIScriptComponent& aiScript, float deltaTime);
    void handleFleeLogic(TransformComponent& transform, TransformComponent& playerTransform, AIScriptComponent& aiScript, float deltaTime);
    void handleDestroyedLogic(TransformComponent& transform, TransformComponent& playerTransform, AIScriptComponent& aiScript, float deltaTime);

    void move(TransformComponent& transform, glm::vec3 destination, float deltaTime);
    void handlePlayerSpottedAlarm(const EnemySpottedPlayerEvent& event, EnttFacade& entt);

    //temporary
    float detectionRadius = 50.0f;
    float attackDistance = 10.0f;
    float movementSpeed = 10.0f;
    int m_parameters = 0;
};



#endif //ENEMYACTIONSYSTEM_H
