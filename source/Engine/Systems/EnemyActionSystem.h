//
// Created by Hugo on 15/05/2025.
//

#ifndef ENEMYACTIONSYSTEM_H
#define ENEMYACTIONSYSTEM_H
#include "EventSystem.h"
#include "Components/AIScriptComponent.h"
#include "Components/TransformComponent.h"
#include "ResourceManagement/EnttFacade.h"


class EnemyActionSystem {
public:
    EnemyActionSystem() = default;
    ~EnemyActionSystem() = default;

    void initialise(EnttFacade& entt);
    void update(float deltaTime, EnttFacade& entt);
private:
    void updateOneEnemy(TransformComponent& transform, TransformComponent& playerTransform, AIScriptComponent& aiScript, float deltaTime);
    void handlePatrolLogic(TransformComponent& transform, TransformComponent& playerTransform, AIScriptComponent& aiScript, float deltaTime);
    void handleAttackLogic(TransformComponent& transform, TransformComponent& playerTransform, AIScriptComponent& aiScript, float deltaTime);
    void handleFleeLogic(TransformComponent& transform, TransformComponent& playerTransform, AIScriptComponent& aiScript, float deltaTime);
    void move(TransformComponent& transform, glm::vec3 destination, float deltaTime);
    void handlePlayerSpottedAlarm(const EnemySpottedPlayerEvent& event, EnttFacade& entt);

    int m_parameters = 0;
};



#endif //ENEMYACTIONSYSTEM_H
