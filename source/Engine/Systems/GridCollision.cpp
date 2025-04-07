
#include "GridCollision.h"

GridCollision::GridCollision(int numRows, int numCols, float spacing, const std::vector<Vertex>& vertices)
: m_numRows(numRows), m_numCols(numCols), m_spacing(spacing), m_vertices(vertices)
{
    m_halfWidth = ((m_numCols - 1) * m_spacing) / 2.0f;
    m_halfDepth = ((m_numRows - 1) * m_spacing) / 2.0f;
}

float GridCollision::getHeightAt(const glm::vec3& worldPos) const
{
    float relativeX = worldPos.x + m_halfWidth;
    float relativeZ = worldPos.z + m_halfDepth;

    int cellX = static_cast<int>(std::floor(relativeX / m_spacing));
    int cellZ = static_cast<int>(std::floor(relativeZ / m_spacing));

    cellX = std::max(0, std::min(cellX, m_numCols - 2));
    cellZ = std::max(0, std::min(cellZ, m_numRows - 2));

    int gridWidth = m_numCols; 
    int indexBL = cellZ * gridWidth + cellX;
    int indexBR = indexBL + 1;
    int indexTL = indexBL + gridWidth;
    int indexTR = indexTL + 1;

    const Vertex& vBL = m_vertices[indexBL];
    const Vertex& vBR = m_vertices[indexBR];
    const Vertex& vTL = m_vertices[indexTL];
    const Vertex& vTR = m_vertices[indexTR];

    float cellOriginX = cellX * m_spacing;
    float cellOriginZ = cellZ * m_spacing;
    float localX = (relativeX - cellOriginX) / m_spacing;
    float localZ = (relativeZ - cellOriginZ) / m_spacing;

    float hBottom = vBL.position.y + localX * (vBR.position.y - vBL.position.y);
    float hTop = vTL.position.y + localX * (vTR.position.y - vTL.position.y);
    float interpolatedHeight = hBottom + localZ * (hTop - hBottom);

    return interpolatedHeight;
}
