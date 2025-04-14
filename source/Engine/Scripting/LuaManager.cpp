// 2025 Semester 1: Murdoch University
// ICT397 Advanced Game Engine Design and Programming
// Created by Buko in March 2025.
// Assignment 1: Game Engine
// LuaManager Class: Manages the Lua interface

//-------------------------------- Libraries ----------------------------------
#include "LuaManager.h"
#include "ResourceManagement/Scene.h"
#include "Terrain/TerrainFactory.h"
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
// Mainly methods that loads assets are exposed to lua
// INPUT the scene object to be exposed
void LuaManager::registerScene(Scene& scene)
{
    m_lua.new_usertype<Scene>(
            "Scene",
            sol::constructors<Scene()>(),
            // "loadModelToRegistry", &Scene::loadModelToRegistry this has been replaced by the below
            "loadModelToRegistry", &Scene::loadModelEntity,
            "loadBoxModelToRegistry", &Scene::loadCollidableBoxEntity,
            "setEntityPosByName", &Scene::setEntityPosByName
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

std::unique_ptr<Terrain> LuaManager::createTerrainFromConfig()
{
    runScript("GameScripts/GameInitialisation.lua");

    sol::table config = m_lua["terrainConfig"];
    std::string typeStr = config["type"];
    int rows = config["rows"];
    int cols = config["cols"];
    float spacing = config["spacing"];

    TerrainTypeEnum terrainEnum;

    if (typeStr == "Heightmap") terrainEnum = TerrainTypeEnum::HEIGHTMAP;
    else if (typeStr == "Fractal") terrainEnum = TerrainTypeEnum::FRACTAL;
    else if (typeStr == "TexturedHeightmap") terrainEnum = TerrainTypeEnum::TEXTURED_HEIGHTMAP;
    else if (typeStr == "MultiTexturedHeightMap") terrainEnum = TerrainTypeEnum::MULTITEXTURED_HEIGHTMAP;
    else if (typeStr == "TexturedFractal") terrainEnum = TerrainTypeEnum::TEXTURED_FRACTAL;
    else throw std::runtime_error("Unknown terrain type: " + typeStr);

    std::map<std::string, std::string> combinedParams;

        // Extract parameters
    sol::table parameters = config["parameters"];
    for (const auto& pair : parameters)
    {
        std::string key = pair.first.as<std::string>();
        std::string value;

        if (pair.second.is<std::string>())
            value = pair.second.as<std::string>();
        else if (pair.second.is<int>())
            value = std::to_string(pair.second.as<int>());
        else if (pair.second.is<float>())
            value = std::to_string(pair.second.as<float>());
        else if (pair.second.is<double>())
            value = std::to_string(static_cast<float>(pair.second.as<double>()));
        else
            continue;

        combinedParams[key] = value;
    }

    // Extract texture (if applicable)
    if (config["texture"].valid())
    {
        sol::table texture = config["texture"];
        combinedParams["path"] = texture["path"];
        combinedParams["repeatX"] = std::to_string(texture["repeatX"].get_or(1));
        combinedParams["repeatY"] = std::to_string(texture["repeatY"].get_or(1));
    }

    auto terrain = std::make_unique<Terrain>(rows, cols, spacing);
    auto terrainType = TerrainFactory::createTerrainType(terrainEnum, combinedParams);
    terrain->setTerrainType(terrainType);
    terrain->generateTerrain();

    return terrain;
}


//-----------------------------------------------------------------------------
int LuaManager::getTerrainRows() const
{
    return m_lua["terrainConfig"]["rows"].get<int>();
}

//-----------------------------------------------------------------------------
int LuaManager::getTerrainCols() const
{
    return m_lua["terrainConfig"]["cols"].get<int>();
}

//-----------------------------------------------------------------------------
float LuaManager::getTerrainSpacing() const
{
    return m_lua["terrainConfig"]["spacing"].get<float>();
}

std::string LuaManager::getSplashImagePath()
{
    runScript("GameScripts/GameInitialisation.lua");
    return m_lua["img_splashScreen_path"];
}

std::string LuaManager::getHelpManualPath()
{
    runScript("GameScripts/GameInitialisation.lua");
    return m_lua["text_helpManual_path"];
}

glm::vec3 LuaManager::getVec3FromLua(const std::string& tableName) {
    sol::table t = m_lua[tableName];
    return {
        t["x"].get_or(0.0f),
        t["y"].get_or(0.0f),
        t["z"].get_or(0.0f)
    };
}

float LuaManager::getFloatFromLua(const std::string& name) {
    sol::object value = m_lua[name];
    if (value.valid() && value.is<float>()) {
        return value.as<float>();
    } else {
        std::cerr << "[LuaManager] Warning: '" << name << "' not found or not a float. Returning 0.\n";
        return 0.0f;
    }
}

std::string LuaManager::getStringFromLua(const std::string& name) {
    sol::object value = m_lua[name];
    if (value.valid() && value.is<std::string>()) {
        return value.as<std::string>();
    } else {
        std::cerr << "[LuaManager] Warning: '" << name << "' not found or not a string. Returning empty string.\n";
        return "";
    }
}


//====================== END OF LUA MANAGER CLASS =============================