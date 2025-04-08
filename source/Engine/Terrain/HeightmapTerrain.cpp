
#include "HeightmapTerrain.h"

#include <iostream>

#include "texture/TextureLoader.h"

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
        float u = vertex.texCoords.x;
        float v = vertex.texCoords.y;
        
        float sampledHeight = bilinearSample(u, v);

        vertex.position.y = sampledHeight * m_heightScale;
    }
}

void HeightmapTerrain::loadHeightMap(const std::string& heightMapPath)
{
    Texture texture;
    texture.loadImageData(heightMapPath);
    m_heightMapData = texture.getData();
    m_heightMapWidth = texture.getWidth();
    m_heightMapHeight = texture.getHeight();
    m_heightMapChannels = texture.getChannels();
    int length = texture.getWidth() * texture.getHeight() * texture.getChannels();

    std::cout << "Length of the image data buffer is: " << length << std::endl;
    std::cout << "The width is: " << texture.getWidth() << std::endl;
    std::cout << "The height is: " << texture.getHeight() << std::endl;
    std::cout << "The number of channels is: " << texture.getChannels() << std::endl;

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
