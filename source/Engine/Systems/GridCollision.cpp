
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

glm::vec3 GridCollision::getNormalAt(const glm::vec3& worldPos) const
{
    // Convert from world coordinates to grid-local coordinates
    float relativeX = worldPos.x + m_halfWidth;
    float relativeZ = worldPos.z + m_halfDepth;

    // Determine the cell indices
    int cellX = static_cast<int>(std::floor(relativeX / m_spacing));
    int cellZ = static_cast<int>(std::floor(relativeZ / m_spacing));

    // Clamp cell indices so they remain in valid range (avoids reading beyond the grid)
    cellX = std::max(0, std::min(cellX, m_numCols - 2));
    cellZ = std::max(0, std::min(cellZ, m_numRows - 2));

    // Compute indices for the four vertices that form the cell (Bottom-Left, Bottom-Right, Top-Left, Top-Right)
    int gridWidth = m_numCols; 
    int indexBL = cellZ * gridWidth + cellX;
    int indexBR = indexBL + 1;
    int indexTL = indexBL + gridWidth;
    int indexTR = indexTL + 1;

    const Vertex& vBL = m_vertices[indexBL];
    const Vertex& vBR = m_vertices[indexBR];
    const Vertex& vTL = m_vertices[indexTL];
    const Vertex& vTR = m_vertices[indexTR];

    // Determine the local (u, v) coordinates inside this grid cell.
    float cellOriginX = cellX * m_spacing;
    float cellOriginZ = cellZ * m_spacing;
    float localX = (relativeX - cellOriginX) / m_spacing; // u in [0, 1]
    float localZ = (relativeZ - cellOriginZ) / m_spacing; // v in [0, 1]

    // Bilinearly interpolate normals:
    // Interpolate between bottom-left and bottom-right normals
    glm::vec3 normalBottom = glm::mix(vBL.normal, vBR.normal, localX);
    // Interpolate between top-left and top-right normals
    glm::vec3 normalTop = glm::mix(vTL.normal, vTR.normal, localX);
    // Interpolate between the two results along the v coordinate
    glm::vec3 interpolatedNormal = glm::mix(normalBottom, normalTop, localZ);

    // Renormalize the interpolated normal to ensure unit length
    return glm::normalize(interpolatedNormal);
}