/**
 * @file TerrainType.h
 * @brief Abstract base class for terrain shaping strategies.
 * @author Shaun
 * @date March/April 2025
 */

#ifndef TERRAINTYPE_H
#define TERRAINTYPE_H
#include "Components/DataTypes.h"

/**
 * @class TerrainType
 * @brief Interface for terrain modification algorithms.
 *
 * Implementations modify a mesh based on heightmap or procedural logic.
 */
class TerrainType
{
public:
    virtual ~TerrainType() = default;

    /**
     * @brief Apply terrain shaping to the provided mesh data.
     * @param meshData Reference to the raw mesh to deform.
     * @param numRows Number of rows in the terrain grid.
     * @param numCols Number of columns in the terrain grid.
     */
    virtual void apply(RawMeshData& meshData, const int numRows, const int numCols) = 0;
};

#endif //TERRAINTYPE_H
