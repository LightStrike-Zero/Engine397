
#ifndef GRIDCOLLISION_H
#define GRIDCOLLISION_H


#include <vector>
#include <glm/glm.hpp>
#include "Components/DataTypes.h"

class GridCollision {
public:
    GridCollision(int numRows, int numCols, float spacing, const std::vector<Vertex>& vertices);
    float getHeightAt(const glm::vec3& worldPos) const;

private:
    int m_numRows;
    int m_numCols;
    float m_spacing;
    float m_halfWidth;
    float m_halfDepth;
    const std::vector<Vertex>& m_vertices;
};

#endif //GRIDCOLLISION_H
