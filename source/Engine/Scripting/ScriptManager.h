// 2025 Semester 1: Murdoch University
// ICT397 Advanced Game Engine Design and Programming
// Created by Buko in March 2025.
// Assignment 1: Game Engine
// ScriptManager Class Header: Manages the script interface
// Specific script managers such as LuaManager will inherit from this base class

// Functionalities include:
// - register the scene class with the scripting language
// - run the given script
// - send variable data across

/**
 * @file ScriptManager.h
 * @brief Abstract class for any future scripting for the engine.
 * @author Buko
 * @date March 2025
*/

#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H
#include "ResourceManagement/Scene.h"

#include "Terrain/Terrain.h"

class Scene;                            // forward decleration for the pointer used in the parameter

/**
 * @class DirectionalLight
 * @brief A class representing a directional light source.
 * virtual methods as the implementation will happen in child classes
 */
class ScriptManager
{
    public:
        /**
         * @brief Virtual destructor for base script manager.
         */
        virtual ~ScriptManager() = default;

       /**
        * @brief Registers the scene so it can be accessed from the scripting language.
        * @param scene Reference to the current scene.
        */
        virtual void registerScene(Scene& scene) = 0;

       /**
        * @brief Runs a script from the specified path.
        * @param scriptPath File path to the script.
        */
        virtual void runScript(const std::string& scriptPath) = 0;

        /**
         * @brief Creates terrain using script-provided configuration.
         * @return A unique pointer to the generated terrain.
         */
        virtual std::unique_ptr<Terrain> createTerrainFromConfig() = 0;

       /**
        * @brief Gets the terrain row count from script config.
        * @return Number of rows.
        */
        virtual int getTerrainRows() const = 0;

       /**
        * @brief Gets the terrain column count from script config.
        * @return Number of columns.
        */
        virtual int getTerrainCols() const = 0;

       /**
        * @brief Gets terrain spacing from the script config.
        * @return Spacing between vertices.
        */
        virtual float getTerrainSpacing() const = 0;

       /**
        * @brief Gets the path to the splash image from the script config.
        * @return Splash image path string.
        */
        virtual std::string getSplashImagePath() = 0;

       /**
        * @brief Gets the path to the help manual file from the script config.
        * @return Help manual file path string.
        */
        virtual std::string getHelpManualPath() = 0;

        /**
         * grabs a vec3 from lua
         * @param tableName
         * @return a vec3 defined in lua
         */
        virtual glm::vec3 getVec3FromLua(const std::string& tableName) = 0;
        /**
         * grabs a float from lua
         * @param name
         * @return a float defined in lua
         */
        virtual float getFloatFromLua(const std::string& name) = 0;
        virtual std::string getStringFromLua(const std::string& name) = 0;

        virtual std::array<std::string, 6> getSkyboxFaces() = 0;

 
};

#endif //SCRIPTMANAGER_H
