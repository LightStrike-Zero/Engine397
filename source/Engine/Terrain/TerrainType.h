

#ifndef TERRAINTYPE_H
#define TERRAINTYPE_H
#include "Components/DataTypes.h"

class TerrainType
{
public:
    virtual ~TerrainType() = default;

    virtual void apply(RawMeshData& meshData, const int numRows, const int numCols) = 0;
};

#endif //TERRAINTYPE_H
