//
// Created by buko on 27/03/2025.
//

#ifndef LUAMANAGER_H
#define LUAMANAGER_H

#include <sol/sol.hpp>
#include <iostream>

class Scene;

class LuaManager{

public:
    LuaManager();
    ~LuaManager();

    void registerScene(Scene& scene); // Expose Scene to Lua
    void runScript(const std::string& scriptPath);

private:
    sol::state m_lua; // Lua state
};

#endif //LUAMANAGER_H
