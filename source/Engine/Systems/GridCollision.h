/**
 * @file GridCollision.h
 * @brief Provides height-based collision detection using a terrain grid.
 * @author Shaun
 * @date March 2025
 */

#ifndef GRIDCOLLISION_H
#define GRIDCOLLISION_H


#include <vector>
#include <glm/glm.hpp>
#include "Components/DataTypes.h"

/**
 * @class GridCollision
 * @brief Class that enables terrain-based collision detection by sampling height from a vertex grid.
 */
class GridCollision {
public:
    /**
     * @brief Constructor to initialize the grid and associated terrain data.
     * @param numRows Number of rows in the terrain grid.
     * @param numCols Number of columns in the terrain grid.
     * @param spacing Distance between vertices in the grid.
     * @param vertices Reference to a vector of terrain vertices.
     */
    GridCollision(int numRows, int numCols, float spacing, const std::vector<Vertex>& vertices);

    /**
     * @brief Gets the height value at a given world position.
     * @param worldPos The position in world coordinates.
     * @return The interpolated height value at the specified location.
     */
    float getHeightAt(const glm::vec3& worldPos) const;

private:
    int m_numRows;                                  ///< Number of grid rows
    int m_numCols;                                  ///< Number of grid columns
    float m_spacing;                                ///< Distance between grid vertices
    float m_halfWidth;                              ///< Half the width of the grid, used for centering
    float m_halfDepth;                              ///< Half the depth of the grid, used for centering
    const std::vector<Vertex>& m_vertices;          ///< Reference to terrain vertex data
};

#endif //GRIDCOLLISION_H
