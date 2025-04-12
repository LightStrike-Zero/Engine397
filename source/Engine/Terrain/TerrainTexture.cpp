
#include "TerrainTexture.h"

#include <iostream>
#include <Texture/TextureManager.h>

#include "Interfaces/ITexture.h"
#include "texture/TextureLoader.h"

TerrainTexture::TerrainTexture(std::shared_ptr<TerrainType> base, const std::string& texturePath, int repeatX, int repeatY)
        : m_wrappedTerrainType(base), m_texturePath(texturePath), m_repeatX(repeatX), m_repeatY(repeatY)
{

}

void TerrainTexture::apply(RawMeshData& meshData, int numRows, int numCols)
{
    loadTerrainTexture(meshData, m_texturePath);

    if (m_wrappedTerrainType)
    {
        m_wrappedTerrainType->apply(meshData, numRows, numCols);
    }

    // Apply UV tiling - buko
    for (auto& vertex : meshData.vertices)
    {
        vertex.texCoords.x *= static_cast<float>(m_repeatX);
        vertex.texCoords.y *= static_cast<float>(m_repeatY);
    }
}

void TerrainTexture::loadTerrainTexture(RawMeshData& meshData, const std::string& texturePath)
{
    try {
        // Use TextureManager to load the texture from file.
        // This returns a GPU texture ID (e.g., a uint32_t).
        uint32_t textureID = TextureManager::getInstance().loadTextureFromFile(texturePath);
        std::cout << "Loaded texture from: " << texturePath << std::endl;
        meshData.material.baseColorTextureID = textureID;
        std::cout << "Texture ID: " << textureID << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to load terrain texture: " << e.what() << std::endl;
    }
}