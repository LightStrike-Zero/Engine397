// 2025 Semester 1: Murdoch University
// ICT397 Advanced Game Engine Design and Programming
// Created by Buko in March 2025.
// Assignment 1: Game Engine
// LuaManager Class Header: Manages the Lua interface

#ifndef LUAMANAGER_H
#define LUAMANAGER_H

#include <sol/sol.hpp>
#include <iostream>

class Scene;

class LuaManager{

public:
    LuaManager();
    ~LuaManager();

    void registerScene(Scene& scene);              // Expose Scene to Lua
    void runScript(const std::string& scriptPath); // runs the Lua script

private:
    sol::state m_lua;                              // Lua state
};

#endif //LUAMANAGER_H
