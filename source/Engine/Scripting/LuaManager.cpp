// 2025 Semester 1: Murdoch University
// ICT397 Advanced Game Engine Design and Programming
// Created by Buko in March 2025.
// Assignment 1: Game Engine
// LuaManager Class: Manages the Lua interface

#include "LuaManager.h"
#include <iostream>
#include <fstream>

//--------------------------- Lua Manager Constructor -------------------------
// base library required
// math and string libraries may be needed in teh future
LuaManager::LuaManager()
{
    m_lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);
}

//--------------------------- Lua Manager Destructor --------------------------
LuaManager::~LuaManager()
{
    // Lua cleanup (handled automatically by sol2)
}

//------------------------ Register Scene Method ------------------------------
// Registers the class scene with lua so it can be exposed to the script
// Currently only loadModelToRegistry method is exposed to Lua
// INPUT the scene object to be exposed
void LuaManager::registerScene(Scene& scene)
{
    m_lua.new_usertype<Scene>(
            "Scene",
            sol::constructors<Scene()>(),
            // "loadModelToRegistry", &Scene::loadModelToRegistry this has been replaced by the below
            "loadModelToRegistry", &Scene::loadModelEntity
    );

    // Pass the Scene instance to Lua using Sol2
    m_lua["scene"] = &scene;
}

//----------------------- Run Script Method -----------------------------------
// This runs the Lua script
// INPUT: script file path as a string
void LuaManager::runScript(const std::string& scriptPath)
{
    try
    {
        m_lua.script_file(scriptPath);
    }
    catch (const sol::error& e)
    {
        std::cerr << "Whoopsie: Lua File Error: " << e.what() << std::endl;
    }
}

//====================== END OF LUA MANAGER CLASS =============================