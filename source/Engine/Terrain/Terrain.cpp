
#include "Terrain.h"

#include <iostream>
#include <vector>
#include <glm/ext/quaternion_common.hpp>

Terrain::Terrain(const int numRows, const int numCols, const float spacing)
    : m_numRows(numRows), m_numCols(numCols), m_spacing(spacing), m_terrainType(nullptr)
{
    generateGrid();
}

void Terrain::generateTerrain()
{
    generateGrid();
    if (m_terrainType)
    {
        m_terrainType->apply(m_meshData, m_numRows, m_numCols);
    }
}

void Terrain::setTerrainType(const std::shared_ptr<TerrainType>& terrainType)
{
    m_terrainType = terrainType;
}

void Terrain::generateGrid() {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    float halfWidth = ((m_numCols - 1) * m_spacing) / 2.0f;
    float halfDepth = ((m_numRows - 1) * m_spacing) / 2.0f;

    vertices.resize(m_numRows * m_numCols);
    for (int row = 0; row < m_numRows; ++row) {
        for (int col = 0; col < m_numCols; ++col) {
            Vertex vertex;
            vertex.position = glm::vec3(col * m_spacing - halfWidth, 0.0f, row * m_spacing - halfDepth);
            vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
            vertex.texCoords = glm::vec2(static_cast<float>(col) / (m_numCols - 1),
                                         static_cast<float>(row) / (m_numRows - 1));
            vertices[row * m_numCols + col] = vertex;
        }
    }

    indices.reserve((m_numRows - 1) * (m_numCols - 1) * 6);
    for (int row = 0; row < m_numRows - 1; ++row) {
        for (int col = 0; col < m_numCols - 1; ++col) {
            int topLeft     = row * m_numCols + col;
            int topRight    = topLeft + 1;
            int bottomLeft  = (row + 1) * m_numCols + col;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    m_meshData.vertices = vertices;
    m_meshData.indices = indices;
    m_meshData.transform = glm::mat4(1.0f);  // Identity transform.
}
