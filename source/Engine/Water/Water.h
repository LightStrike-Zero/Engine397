/**
 * @file Water.h
 * @brief Declares the Water class responsible for generating water surface meshes.
 *
 * This class handles the generation of a flat water surface mesh at a specified height.
 * It follows a similar pattern to the Terrain class but simplified for water surfaces.
 *
 * @author Shaun
 * @date May 2025
 */

#ifndef WATER_H
#define WATER_H
#include <memory>
#include "Components/DataTypes.h"

/**
 * @class Water
 * @brief Manages the water surface mesh data.
 *
 * This class creates and stores mesh data for a water surface based on rows, columns,
 * spacing, and a specific water level height.
 */
class Water
{
public:
    /**
     * @brief Constructs a water surface grid with given dimensions and spacing.
     * @param numRows Number of rows in the grid.
     * @param numCols Number of columns in the grid.
     * @param waterHeight The Y-coordinate height of the water surface.
     * @param spacing Distance between grid points.
     */
    Water(int numRows, int numCols, float waterHeight = 0.0f, float spacing = 0.5f);

    /**
     * @brief Virtual destructor.
     */
    virtual ~Water() = default;

    /**
     * @brief Regenerates the water surface mesh.
     */
    void generateWater();

    /**
     * @brief Sets the water surface height.
     * @param height New height for the water surface.
     */
    void setWaterHeight(float height);

    /**
     * @brief Returns the generated mesh data.
     * @return Const reference to mesh data.
     */
    const RawMeshData& getMeshData() const
    {
        return m_meshData;
    }

protected:
    RawMeshData m_meshData;      ///< Mesh data for the water surface
    float m_spacing;             ///< Distance between vertices
    float m_waterHeight;         ///< Y-coordinate height of the water surface
    int m_numRows;               ///< Number of rows in the grid
    int m_numCols;               ///< Number of columns in the grid

    /**
     * @brief Internal grid generation based on number of rows, columns, spacing, and water height.
     */
    void generateGrid();
};

#endif //WATER_H