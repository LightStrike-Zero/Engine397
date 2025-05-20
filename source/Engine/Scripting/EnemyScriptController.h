//
// Created by Hugo on 16/05/2025.
//

#ifndef ENEMYSCRIPTCONTROLLER_H
#define ENEMYSCRIPTCONTROLLER_H
#include "ScriptController.h"
#include "Systems/EnemyScriptSystem.h"


class EnemyScriptController: public ScriptController {
public:
    EnemyScriptController() = default;

    // void registerClass(EnemyScriptSystem & enemyActionSystem);
    void initialize(EnttFacade& entt);
    void update(EnttFacade& entt, float deltaTime);
private:
    void exposeClassesToLua();
    void getEnemiesTable(EnttFacade& entt);
    static glm::vec3 getPlayerPos(EnttFacade& entt);

};



#endif //ENEMYSCRIPTCONTROLLER_H
