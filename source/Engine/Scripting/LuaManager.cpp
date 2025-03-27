//
// Created by buko on 27/03/2025.
//

#include "LuaManager.h"
#include "Scene.h"
#include <iostream>
#include <fstream>

LuaManager::LuaManager()
{
    m_lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);
}

LuaManager::~LuaManager() {}

void LuaManager::registerScene(Scene& scene)
{
    m_lua.new_usertype<Scene>(
            "Scene",
            sol::constructors<Scene()>(),
            "loadModelToRegistry", &Scene::loadModelToRegistry
    );

    // Pass the Scene instance to Lua
    m_lua["scene"] = &scene;
}

void LuaManager::runScript(const std::string& scriptPath)
{
    try {
        m_lua.script_file(scriptPath);
    } catch (const sol::error& e) {
        std::cerr << "Lua Error: " << e.what() << std::endl;
    }
}