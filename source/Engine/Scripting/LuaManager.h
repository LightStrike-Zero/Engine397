// 2025 Semester 1: Murdoch University
// ICT397 Advanced Game Engine Design and Programming
// Created by Buko in March 2025.
// Assignment 1: Game Engine
// LuaManager Class Header: Manages the Lua interface

#ifndef LUAMANAGER_H
#define LUAMANAGER_H

#include <sol/sol.hpp>
#include <iostream>
#include "ScriptManager.h"
#include "Terrain/Terrain.h"

class Scene;

class LuaManager : public ScriptManager
{
    public:
        LuaManager();
        ~LuaManager();

        void registerScene(Scene& scene) override;              // Expose Scene to Lua
        void runScript(const std::string& scriptPath) override; // runs the Lua script
        sol::table getTerrainConfig() const;                    // grabs the terrain config data
        std::unique_ptr<Terrain> createTerrainFromConfig();
        int getTerrainRows() const;
        int getTerrainCols() const;
        float getTerrainSpacing() const;

    private:
        sol::state m_lua;                              // Lua state

        //HELPERS
        //std::map<std::string, std::string> extractTextureParams(const sol::table& config);
};

#endif //LUAMANAGER_H
