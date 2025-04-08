
#include "TerrainTexture.h"

#include <iostream>

#include "texture/TextureLoader.h"

//TerrainTexture::TerrainTexture(std::shared_ptr<TerrainType> wrappedTerrainType, const std::string& texturePath)
//    : m_wrappedTerrainType(wrappedTerrainType), m_texturePath(texturePath)
//{
//}

TerrainTexture::TerrainTexture(std::shared_ptr<TerrainType> base, const std::string& texturePath, int repeatX, int repeatY)
        : m_wrappedTerrainType(base), m_texturePath(texturePath), m_repeatX(repeatX), m_repeatY(repeatY)
{

}

void TerrainTexture::apply(RawMeshData& meshData, int numRows, int numCols)
{
    loadTerrainTexture(meshData, m_texturePath);
    if (m_wrappedTerrainType) {
        m_wrappedTerrainType->apply(meshData, numRows, numCols);
    }
    
}

void TerrainTexture::loadTerrainTexture(RawMeshData& meshData, const std::string& texturePath)
{
    try {
        auto* texture = new Texture(texturePath);
        meshData.material.baseColorTextureID = texture->getID();
        std::cout << "Texture ID: " << meshData.material.baseColorTextureID << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Failed to load terrain texture: " << e.what() << std::endl;
    }
}