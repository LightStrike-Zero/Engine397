/**
 * @file TerrainFactory.h
 * @brief Provides static factory methods to create various TerrainType instances using configuration parameters.
 * @author Shaun
 * @date March/April 2025
 */

#ifndef TERRAINFACTORY_H
#define TERRAINFACTORY_H

#include "TerrainType.h"
#include <memory>
#include <string>

#include "Terrain.h"   // Buko
#include <sol/sol.hpp> // Buko

#include <map>

/**
 * @enum TerrainTypeEnum
 * @brief Enumerates the supported terrain types.
 */
enum class TerrainTypeEnum
{
    HEIGHTMAP, ///< Load terrain from a heightmap image.
    FRACTAL, ///< Generate terrain using a fractal faulting algorithm.
    TEXTURED_HEIGHTMAP, ///< Heightmap with texture.
    TEXTURED_FRACTAL, ///< Procedural terrain with texture.
    MULTITEXTURED_HEIGHTMAP, ///< Heightmap with multiple textures.
};

/**
 * @class TerrainFactory
 * @brief Factory for constructing different TerrainType strategies based on config.
 */
class TerrainFactory
{
public:
    /**
     * @brief Creates a TerrainType based on enum and optional parameters.
     * @param type Enum value for terrain strategy.
     * @param params Key-value map of parameters.
     * @return A shared pointer to the created TerrainType.
     */
    static std::shared_ptr<TerrainType> createTerrainType(TerrainTypeEnum type,
                                                          const std::map<std::string, std::string>& params = {});

private:
    static std::shared_ptr<TerrainType> createHeightmapTerrain(const std::map<std::string, std::string>& params);
    static std::shared_ptr<TerrainType> createFractalTerrain(const std::map<std::string, std::string>& params);
    static std::shared_ptr<TerrainType> createTexturedTerrain(std::shared_ptr<TerrainType> baseTerrain,
                                                              const std::map<std::string, std::string>& params);
    static std::shared_ptr<TerrainType> createMultiTexturedTerrain(std::shared_ptr<TerrainType> baseTerrain,
                                                                   const std::map<std::string, std::string>& params);
};

#endif //TERRAINFACTORY_H
