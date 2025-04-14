
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

    generateNormals(meshData);

}

void HeightmapTerrain::loadHeightMap(const std::string& heightMapPath)
{
    OpenGLTexture image;
    image.loadImageData(heightMapPath);
    m_heightMapData = image.getData();
    m_heightMapWidth = image.getWidth();
    m_heightMapHeight = image.getHeight();
    m_heightMapChannels = image.getChannels();
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

void HeightmapTerrain::generateNormals(RawMeshData& meshData)
{
    for (auto& vertex : meshData.vertices) {
        vertex.normal = glm::vec3(0.0f);
    }


    for (size_t i = 0; i < meshData.indices.size(); i += 3) {
        const unsigned int index0 = meshData.indices[i];
        const unsigned int index1 = meshData.indices[i + 1];
        const unsigned int index2 = meshData.indices[i + 2];

        Vertex& v0 = meshData.vertices[index0];
        Vertex& v1 = meshData.vertices[index1];
        Vertex& v2 = meshData.vertices[index2];

        glm::vec3 edge1 = v1.position - v0.position;
        glm::vec3 edge2 = v2.position - v0.position;

        glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

        v0.normal += faceNormal;
        v1.normal += faceNormal;
        v2.normal += faceNormal;
    }

    for (auto& vertex : meshData.vertices) {
        vertex.normal = glm::normalize(vertex.normal);
    }
}