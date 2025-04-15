
#include "HeightmapTerrain.h"

#include <iostream>

#include "Interfaces/ITexture.h"
#include "texture/TextureLoader.h"
#include "Texture/OpenGL/OGL_Texture.h"

HeightmapTerrain::HeightmapTerrain(const std::string& heightMapPath, float heightScale)
    : m_heightmapPath(heightMapPath), m_heightScale(heightScale),
      m_heightMapData(nullptr), m_heightMapWidth(0), m_heightMapHeight(0), m_heightMapChannels(0)
{
    loadHeightMap(heightMapPath);
}

void HeightmapTerrain::apply(RawMeshData& meshData, const int numRows, const int numCols)
{
    if (m_heightMapData == nullptr)
    {
        std::cerr << "Heightmap data is null" << std::endl;
        return;
    }
    
    for (auto& vertex : meshData.vertices)
    {
        const float u = vertex.texCoords.x;
        const float v = vertex.texCoords.y;

        const float sampledHeight = bilinearSample(u, v);
        vertex.position.y = sampledHeight * m_heightScale;
    }
    smoothVertexPositions(meshData, numRows, numCols);
    computeMeshGradientNormals(meshData, numRows, numCols);
}

void HeightmapTerrain::loadHeightMap(const std::string& heightMapPath)
{
    OpenGLTexture image;
    image.loadImageData(heightMapPath);
    m_heightMapData = image.getData();
    m_heightMapWidth = image.getWidth();
    m_heightMapHeight = image.getHeight();
    m_heightMapChannels = image.getChannels();

    smoothHeightmapData();
}


float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

float HeightmapTerrain::getPixelAsFloat(float x, float y)
{
    const float position = (x + m_heightMapWidth * y) * m_heightMapChannels;
    return static_cast<float>(m_heightMapData[static_cast<int>(position)]) / 255.0f;
}

//TODO might move this into the texture file as a helper
float HeightmapTerrain::bilinearSample(float xf, float yf)
{
    const float w = m_heightMapWidth - 1;
    const float h = m_heightMapHeight - 1;

    const float x1 = glm::floor(xf * w);
    const float y1 = glm::floor(yf * h);
    const float x2 = glm::clamp(x1 + 1, 0.0f, w);
    const float y2 = glm::clamp(y1 + 1, 0.0f, h);

    const float xp = xf * w - x1;
    const float yp = yf * h - y1;

    const float p11 = getPixelAsFloat(x1, y1);
    const float p21 = getPixelAsFloat(x2, y1);
    const float p12 = getPixelAsFloat(x1, y2);
    const float p22 = getPixelAsFloat(x2, y2);

    const float px1 = lerp(p11, p21, xp);
    const float px2 = lerp(p12, p22, xp);

    return lerp(px1, px2, yp);
}

void HeightmapTerrain::computeMeshGradientNormals(RawMeshData& meshData, int numRows, int numCols) {
    for (auto& vertex : meshData.vertices) {
        vertex.normal = glm::vec3(0.0f);
    }
    
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            int idx = row * numCols + col;
            
            if (row > 0 && row < numRows - 1 && col > 0 && col < numCols - 1) {
                float h_left  = meshData.vertices[(row) * numCols + (col-1)].position.y;
                float h_right = meshData.vertices[(row) * numCols + (col+1)].position.y;
                float h_up    = meshData.vertices[(row-1) * numCols + (col)].position.y;
                float h_down  = meshData.vertices[(row+1) * numCols + (col)].position.y;
                
                float spacing = meshData.vertices[row * numCols + col + 1].position.x - 
                               meshData.vertices[row * numCols + col].position.x;
                
                glm::vec3 tangent_x(2.0f * spacing, h_right - h_left, 0.0f);
                glm::vec3 tangent_z(0.0f, h_down - h_up, 2.0f * spacing);
                
                meshData.vertices[idx].normal = glm::normalize(glm::cross(tangent_z, tangent_x));
            }
        }
    }
    
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            int idx = row * numCols + col;
            
            if (row == 0) {
                meshData.vertices[idx].normal = meshData.vertices[(row+1) * numCols + col].normal;
            }
            else if (row == numRows - 1) {
                meshData.vertices[idx].normal = meshData.vertices[(row-1) * numCols + col].normal;
            }
            else if (col == 0) {
                meshData.vertices[idx].normal = meshData.vertices[row * numCols + (col+1)].normal;
            }
            else if (col == numCols - 1) {
                meshData.vertices[idx].normal = meshData.vertices[row * numCols + (col-1)].normal;
            }
        }
    }
}


void HeightmapTerrain::smoothHeightmapData() {
    if (!m_heightMapData || m_heightMapWidth <= 2 || m_heightMapHeight <= 2) return;
    
    unsigned char* smoothedData = new unsigned char[m_heightMapWidth * m_heightMapHeight * m_heightMapChannels];
    
    for (int y = 0; y < m_heightMapHeight; y++) {
        for (int x = 0; x < m_heightMapWidth; x++) {
            float sum = 0.0f;
            float count = 0.0f;
            
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int nx = x + dx;
                    int ny = y + dy;
                    
                    if (nx >= 0 && nx < m_heightMapWidth && ny >= 0 && ny < m_heightMapHeight) {
                        int pos = (ny * m_heightMapWidth + nx) * m_heightMapChannels;
                        sum += m_heightMapData[pos];
                        count++;
                    }
                }
            }
            
            int pos = (y * m_heightMapWidth + x) * m_heightMapChannels;
            smoothedData[pos] = static_cast<unsigned char>(sum / count);
            
            for (int c = 1; c < m_heightMapChannels; c++) {
                smoothedData[pos + c] = m_heightMapData[pos + c];
            }
        }
    }
    
    delete[] m_heightMapData;
    m_heightMapData = smoothedData;
}

void HeightmapTerrain::smoothVertexPositions(RawMeshData& meshData, int numRows, int numCols)
{
    std::vector<glm::vec3> smoothedPositions;
    smoothedPositions.resize(meshData.vertices.size());
    
    for (size_t i = 0; i < meshData.vertices.size(); i++) {
        smoothedPositions[i] = meshData.vertices[i].position;
    }
    
    const float kernel[3][3] = {
        {0.0625f, 0.125f, 0.0625f},
        {0.125f, 0.25f, 0.125f},
        {0.0625f, 0.125f, 0.0625f}
    };
    
    for (int row = 1; row < numRows - 1; row++) {
        for (int col = 1; col < numCols - 1; col++) {
            int idx = row * numCols + col;
            glm::vec3 weightedSum(0.0f);
            
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int nidx = (row + dy) * numCols + (col + dx);
                    weightedSum += meshData.vertices[nidx].position * kernel[dy+1][dx+1];
                }
            }
            
            smoothedPositions[idx].y = weightedSum.y;
        }
    }
    
    for (size_t i = 0; i < meshData.vertices.size(); i++) {
        meshData.vertices[i].position.y = smoothedPositions[i].y;
    }
}