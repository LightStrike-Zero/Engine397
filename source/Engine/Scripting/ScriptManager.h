// 2025 Semester 1: Murdoch University
// ICT397 Advanced Game Engine Design and Programming
// Created by Buko in March 2025.
// Assignment 1: Game Engine
// ScriptManager Class Header: Manages the script interface
// Specific script managers such as LuaManager will inherit from this base class

// Currently only handling two functionnalities. More will be added as required
// - register the scene class with the scripting language
// - run the given script

#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H
#include "ResourceManagement/Scene.h"

#include "Terrain/Terrain.h"

class Scene;

// virtual methods as the implementation will happen in child classes
class ScriptManager
{
    public:
        virtual ~ScriptManager() = default;

        virtual void runScript(const std::string& scriptPath) = 0;
        virtual void registerScene(Scene& scene) = 0;
        virtual std::unique_ptr<Terrain> createTerrainFromConfig() = 0;
        virtual int getTerrainRows() const = 0;
        virtual int getTerrainCols() const = 0;
        virtual float getTerrainSpacing() const = 0;
        virtual std::string getSplashImagePath() = 0;
        virtual std::string getHelpManualPath() = 0;
};

#endif //SCRIPTMANAGER_H
