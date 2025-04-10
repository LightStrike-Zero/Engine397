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

