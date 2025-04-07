
#ifndef TERRAIN_H
#define TERRAIN_H
#include <memory>
#include "Components/DataTypes.h"
#include "TerrainType.h"


class Terrain
{
public:
    Terrain(const int numRows, const int numCols, float spacing = 0.5f);
    virtual ~Terrain() = default;

    void generateTerrain();

    void setTerrainType(const std::shared_ptr<TerrainType>& terrainType);
    
    const RawMeshData& getMeshData() const
    {
        return m_meshData;
    }

protected:
    RawMeshData m_meshData;
    float m_spacing;
    int m_numRows;
    int m_numCols;

    std::shared_ptr<TerrainType> m_terrainType;
    void generateGrid();
};


#endif //TERRAIN_H
