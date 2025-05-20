//
// Created by Hugo on 16/05/2025.
//

#include "EnemyScriptController.h"

#include "Components/EnemyComponent.h"
#include "Components/PlayerControllerComponent.h"

// void EnemyScriptController::registerClass(EnemyScriptSystem & enemyActionSystem) {
//     m_lua.new_usertype<EnemyScriptSystem>(
//         "EnemyScriptSystem",
//         sol::constructors<EnemyScriptSystem()>()
//     );
// }

void EnemyScriptController::initialize(EnttFacade& entt) {
    m_lua.open_libraries(sol::lib::base,
        sol::lib::math,
        sol::lib::string,
        sol::lib::os,
        sol::lib::package,
        sol::lib::table
    );
    exposeClassesToLua();
    try {
        m_lua.script_file("GameScripts/FSMTesting2.lua");
    } catch (const sol::error& e) {
        std::cerr << "Failed to load Lua script: " << e.what() << std::endl;
    }
    // m_lua.script_file("../GameScripts/FSMTesting2.lua");
    getEnemiesTable(entt);
    m_lua["initEnemies"]();

}

void EnemyScriptController::update(EnttFacade& entt, float deltaTime) {
    m_lua["playerPos"] = getPlayerPos(entt);
    m_lua["Update"](deltaTime);
    auto view = entt.view<TransformComponent,AIScriptComponent,EnemyComponent>();
    for (auto entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        std::cout << "enemy pos:" << std::endl;
        std::cout << transform.position.x << " " << transform.position.y << " " << transform.position.z << std::endl;
    }
}

void EnemyScriptController::exposeClassesToLua() {
    m_lua.new_usertype<glm::vec3>("vec3",
        sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(),
        "x", &glm::vec3::x,
        "y", &glm::vec3::y,
        "z", &glm::vec3::z
    );

    m_lua.set_function("makeVec3", [](float x, float y, float z) {
        return glm::vec3(x, y, z);
    });

    m_lua.new_usertype<TransformComponent>("TransformComponent",
        "position", &TransformComponent::position,
        "rotation", &TransformComponent::rotation,
        "scale", &TransformComponent::scale
    );
    //destroyed state
}


glm::vec3 EnemyScriptController::getPlayerPos(EnttFacade& entt) {
    auto playerView = entt.view<TransformComponent, PlayerControllerComponent>();
    auto& playerTransform = playerView.get<TransformComponent>(playerView.front());
    return playerTransform.position;
}
//get the player transform


//scan and pull out all enemies
void EnemyScriptController::getEnemiesTable(EnttFacade& entt) {
    sol::table enemies = m_lua.create_table();
    int index = 1;
    auto view = entt.view<TransformComponent,AIScriptComponent,EnemyComponent>();
    for (auto entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        enemies[index] = &transform;
        index++;
    }
    m_lua["enemies"] = enemies;
}