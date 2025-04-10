#include "TerrainFactory.h"
#include "HeightmapTerrain.h"
#include "FractalTerrain.h"
#include "TerrainTexture.h"

#include <iostream>

std::shared_ptr<TerrainType> TerrainFactory::createTerrainType(TerrainTypeEnum type, const std::map<std::string, std::string>& params) {
    switch (type) {
        case TerrainTypeEnum::HEIGHTMAP:
            return createHeightmapTerrain(params);

        case TerrainTypeEnum::FRACTAL:
            return createFractalTerrain(params);

        case TerrainTypeEnum::TEXTURED_HEIGHTMAP: {
            auto baseTerrain = createHeightmapTerrain(params);
            return createTexturedTerrain(baseTerrain, params);
        }

        case TerrainTypeEnum::TEXTURED_FRACTAL: {
            auto baseTerrain = createFractalTerrain(params);
            return createTexturedTerrain(baseTerrain, params);
        }

        default:
            std::cerr << "[DEBUG] Unknown terrain type requested" << std::endl;
            return nullptr;
    }
}

//---------------- PRIVATE HELPERS ------------------
std::shared_ptr<TerrainType> TerrainFactory::createHeightmapTerrain(const std::map<std::string, std::string>& params) {
    std::string heightmapPath = "assets/heightmap.png";
    float heightScale = 1.0f;

    if (params.count("heightmapPath"))
        heightmapPath = params.at("heightmapPath");

    if (params.count("heightScale"))
        heightScale = std::stof(params.at("heightScale"));

    return std::make_shared<HeightmapTerrain>(heightmapPath, heightScale);
}

std::shared_ptr<TerrainType> TerrainFactory::createFractalTerrain(const std::map<std::string, std::string>& params) {
    int iterations = 8;
    float initialDisplacement = 0.5f;
    float displacementDecay = 0.65f;
    float heightScale = 1.0f;
    float smoothness = 1.0f;
    int smoothingPasses = 1;
    unsigned int seed = 123456789;

    if (params.count("iterations"))
        iterations = std::stoi(params.at("iterations"));

    if (params.count("initialDisplacement"))
        initialDisplacement = std::stof(params.at("initialDisplacement"));

    if (params.count("displacementDecay"))
        displacementDecay = std::stof(params.at("displacementDecay"));

    if (params.count("heightScale"))
        heightScale = std::stof(params.at("heightScale"));

    if (params.count("seed"))
        seed = std::stoi(params.at("seed"));

    if (params.count("smoothness"))
        smoothness = std::stof(params.at("smoothness"));

    if (params.count("smoothingPasses"))
        smoothingPasses = std::stoi(params.at("smoothingPasses"));

    return std::make_shared<FractalTerrain>(iterations, initialDisplacement, displacementDecay, heightScale, seed, smoothness, smoothingPasses);
}

std::shared_ptr<TerrainType> TerrainFactory::createTexturedTerrain(std::shared_ptr<TerrainType> baseTerrain, const std::map<std::string, std::string>& params) {
    std::string texturePath = params.at("path");  // now mandatory
    int repeatX = std::stoi(params.at("repeatX"));
    int repeatY = std::stoi(params.at("repeatY"));

    return std::make_shared<TerrainTexture>(baseTerrain, texturePath, repeatX, repeatY);
}

// The one below was the original createTexturedTerrain, the one above handles tiling

//std::shared_ptr<TerrainType> TerrainFactory::createTexturedTerrain(std::shared_ptr<TerrainType> baseTerrain, const std::map<std::string, std::string>& params) {
//    std::string texturePath = "assets/stone.png"; // Default value
//
//    if (params.count("texturePath"))
//        texturePath = params.at("texturePath");
//
//    return std::make_shared<TerrainTexture>(baseTerrain, texturePath);
//}

//// buko -----------------------------------------------------------------------
//// possibly need to abstrcat further so no lua inside this class
//std::unique_ptr<Terrain> TerrainFactory::createFromLuaConfig(const sol::table& config)
//{
//    std::string type = config["type"].get<std::string>();
//    int rows = config["rows"].get<int>();
//    int cols = config["cols"].get<int>();
//    float spacing = config["spacing"].get<float>();
//
//    std::unique_ptr<Terrain> terrain = std::make_unique<Terrain>(rows, cols, spacing);
//    sol::table params = config["parameters"];
//
//    std::shared_ptr<TerrainType> terrainType;
//
//    if (type == "Heightmap" || type == "TexturedHeightmap")
//    {
//        terrainType = std::make_shared<HeightmapTerrain>(
//                params["file"].get<std::string>(),
//                params.get_or("heightScale", 1.0f)
//        );
//    }
//    else if (type == "Fractal" || type == "TexturedFractal")
//    {
//        terrainType = std::make_shared<FractalTerrain>(
//                params["iterations"].get<int>(),
//                params["initialDisplacement"].get<float>(),
//                params["displacementDecay"].get<float>(),
//                params["heightScale"].get<float>(),
//                params["seed"].get<int>(),
//                params["smoothness"].get<float>(),
//                params["smoothingPasses"].get<int>()
//        );
//    }
//    else
//    {
//        throw std::runtime_error("Unsupported terrain type in Lua config: " + type);
//    }
//
//    // Wrap in texture decorator if needed
//    if (type == "TexturedFractal" || type == "TexturedHeightmap")
//    {
//        sol::table tex = config["texture"];
//        std::map<std::string, std::string> texParams;
//        texParams["path"] = tex["path"];
//        texParams["repeatX"] = std::to_string(static_cast<int>(tex["repeatX"]));
//        texParams["repeatY"] = std::to_string(static_cast<int>(tex["repeatY"]));
//
//        terrainType = createTexturedTerrain(terrainType, texParams);
//    }
//
//    terrain->setTerrainType(terrainType);
//    terrain->generateTerrain();
//
//    return terrain;
//}


