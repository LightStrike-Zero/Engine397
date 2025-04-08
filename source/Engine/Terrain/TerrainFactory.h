
#ifndef TERRAINFACTORY_H
#define TERRAINFACTORY_H

#include "TerrainType.h"
#include <memory>
#include <string>

#include "Terrain.h"   // Buko
#include <sol/sol.hpp> // Buko

#include <map>

enum class TerrainTypeEnum {
  HEIGHTMAP,
  FRACTAL,
  TEXTURED_HEIGHTMAP,
  TEXTURED_FRACTAL
};

class TerrainFactory {
public:
  static std::unique_ptr<Terrain> createFromLuaConfig(const sol::table& config); //BUKO
  static std::shared_ptr<TerrainType> createTerrainType(TerrainTypeEnum type, const std::map<std::string, std::string> & params = {});

private:
  static std::shared_ptr<TerrainType> createHeightmapTerrain(const std::map<std::string, std::string>& params);
  static std::shared_ptr<TerrainType> createFractalTerrain(const std::map<std::string, std::string>& params);
  static std::shared_ptr<TerrainType> createTexturedTerrain(std::shared_ptr<TerrainType> baseTerrain, const std::map<std::string, std::string>& params);
};

#endif //TERRAINFACTORY_H
