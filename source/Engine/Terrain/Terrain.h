/**
 * @file Terrain.h
 * @brief Declares the Terrain class responsible for holding terrain mesh data and applying terrain types.
 *
 * This class acts as the main interface for managing terrain generation, storing the mesh data and allowing
 * a `TerrainType` strategy to be applied to shape the terrain. It separates terrain grid generation logic
 * from height/texture manipulation to ensure a flexible and extendable terrain system.
 *
 * @author Shaun
 * @date March/April 2025
 */

#ifndef TERRAIN_H
#define TERRAIN_H
#include <memory>
#include "Components/DataTypes.h"
#include "TerrainType.h"

/**
 * @class Terrain
 * @brief Holds mesh data and delegates terrain shaping to a TerrainType strategy.
 *
 * This class manages the terrain grid structure based on rows, columns, and spacing, and stores mesh data in
 * a `RawMeshData` structure. It supports applying different procedural or image-based terrain types by
 * delegating to a shared `TerrainType` instance.
 */
class Terrain
{
public:

    /**
      * @brief Constructs a grid with given dimensions and spacing.
      * @param numRows Number of rows in the grid.
      * @param numCols Number of columns in the grid.
      * @param spacing Distance between grid points.
      */
    Terrain(const int numRows, const int numCols, float spacing = 0.5f);

    /**
     * @brief Virtual destructor.
     */
    virtual ~Terrain() = default;

    /**
     * @brief Generates the full terrain including grid and heightmap/texturing.
     */
    void generateTerrain();

    /**
     * @brief Sets the terrain shaping strategy.
     * @param terrainType Shared pointer to a `TerrainType` instance.
     */
    void setTerrainType(const std::shared_ptr<TerrainType>& terrainType);

    /**
     * @brief Returns the generated mesh data.
     * @return Const reference to mesh data.
     */
    const RawMeshData& getMeshData() const
    {
        return m_meshData;
    }

protected:
    RawMeshData m_meshData;                              ///< Mesh data for the terrain
    float m_spacing;                                     ///< Distance between vertices
    int m_numRows;                                       ///< Number of rows in the grid
    int m_numCols;                                       ///< Number of columns in the grid

    std::shared_ptr<TerrainType> m_terrainType;          ///< TerrainType strategy used to shape the terrain

    /**
     * @brief Internal grid generation based on number of rows, columns, and spacing.
     */
    void generateGrid();
};

#endif //TERRAIN_H
