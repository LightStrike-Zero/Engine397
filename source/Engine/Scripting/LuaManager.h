// 2025 Semester 1: Murdoch University
// ICT397 Advanced Game Engine Design and Programming
// Created by Buko in March 2025.
// Assignment 1: Game Engine
// LuaManager Class Header: Manages the Lua interface

/**
 * @file LuaManager.h
 * @brief Handles Lua scripting for the engine.
 * This child class is responsible for managing the Lua scripting environment using Sol2.
 * It runs scripts, generates terrain via Lua-defined parameters,
 * and utility access to values and paths in Lua scripts.
 * @author Buko
 * @date March 2025
*/

#ifndef LUAMANAGER_H
#define LUAMANAGER_H

#include <sol/sol.hpp>
#include <iostream>
#include "ScriptManager.h"
#include "ResourceManagement/Scene.h"
#include "Terrain/Terrain.h"

class Scene;

class LuaManager : public ScriptManager
{
    public:
        LuaManager();
        ~LuaManager();

       /**
        * @copydoc ScriptManager::registerScene
        */
        void registerScene(Scene& scene) override;              // Expose Scene to Lua

       /**
        * @copydoc ScriptManager::runScript
        */
        void runScript(const std::string& scriptPath) override; // runs the Lua script

        /**
         * @copydoc ScriptManager::createTerrainFromConfig
         */
        std::unique_ptr<Terrain> createTerrainFromConfig();

       /**
        * @copydoc ScriptManager::getTerrainRows
        */
        int getTerrainRows() const;

       /**
        * @copydoc ScriptManager::getTerrainCols
        */
        int getTerrainCols() const;

       /**
        * @copydoc ScriptManager::getTerrainSpacing
        */
        float getTerrainSpacing() const;

       /**
        * @copydoc ScriptManager::getSplashImagePath
        */
        std::string getSplashImagePath() override;

       /**
        * @copydoc ScriptManager::getHelpManualPath
        */
        std::string LuaManager::getHelpManualPath() override;

    private:
        sol::state m_lua;              // Lua state
};

#endif //LUAMANAGER_H
