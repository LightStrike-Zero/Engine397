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

class Scene;

class LuaManager : public ScriptManager
{
    public:
        LuaManager();
        ~LuaManager();

        void registerScene(Scene& scene) override;              // Expose Scene to Lua
        void runScript(const std::string& scriptPath) override; // runs the Lua script
        sol::table getTerrainConfig() const;                    // grabs the terrain config data

    private:
        sol::state m_lua;                              // Lua state
};

#endif //LUAMANAGER_H
