#include "TerrainMultiTexture.h"

#include <iostream>

#include "Components/DataTypes.h"  
#include "Texture/TextureLoader.h" 
#include <vector>
#include <Texture/TextureManager.h>

TerrainMultiTexture::TerrainMultiTexture(std::shared_ptr<TerrainType> base, const std::string& texturePath, int repeatX, int repeatY)
    : m_wrappedTerrainType(base), m_texturePath(texturePath), m_repeatX(repeatX), m_repeatY(repeatY)
{
}

void TerrainMultiTexture::apply(RawMeshData& meshData, int numRows, int numCols)
{
    loadTerrainMultiTexture(meshData, m_texturePath);
    if (m_wrappedTerrainType)
    {
        m_wrappedTerrainType->apply(meshData, numRows, numCols);
    }
    
}

void TerrainMultiTexture::loadTerrainMultiTexture(RawMeshData& meshData, const std::string& texturePath)
{
    try {
        uint32_t textureID = TextureManager::getInstance().createCompositeTexture(
            { "Assets/Terrain/Textures/Terrain003_2K.png", "Assets/Terrain/Textures/Mountain_01.png", "Assets/Terrain/Textures/Mountain_03.png", "Assets/Terrain/Textures/Mountain_02.png" },
            { 0.02f, 0.7f}
        );
        
        std::cout << "Loaded multi texture from: " << texturePath << std::endl;
        meshData.material.baseColorTextureID = textureID;
        std::cout << "Multi-Texture ID: " << textureID << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to load terrain multi texture: " << e.what() << std::endl;
    }
}
