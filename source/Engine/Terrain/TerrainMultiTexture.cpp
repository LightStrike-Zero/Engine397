#include "TerrainMultiTexture.h"

#include <iostream>

#include "Components/DataTypes.h"  
#include "Texture/TextureLoader.h" 
#include <vector>
#include <Texture/TextureManager.h>

TerrainMultiTexture::TerrainMultiTexture(std::shared_ptr<TerrainType> base, const std::vector<std::string>& texturePath, const std::pair<float, float>& blendParams, int repeatX, int repeatY)
    : m_wrappedTerrainType(base), m_texturePaths(texturePath), m_blendParams(blendParams), m_repeatX(repeatX), m_repeatY(repeatY)
{
}

void TerrainMultiTexture::apply(RawMeshData& meshData, int numRows, int numCols)
{
    loadTerrainMultiTexture(meshData, m_texturePaths, m_blendParams);
    if (m_wrappedTerrainType)
    {
        m_wrappedTerrainType->apply(meshData, numRows, numCols);
    }
    
}

void TerrainMultiTexture::loadTerrainMultiTexture(RawMeshData& meshData, const std::vector<std::string>& texturePath, const std::pair<float, float>& blendParams)
{
    try {
        // path 0 is the heightmap, 1, 2, 3 are the textures followed by the blend parameters
        const uint32_t textureID = TextureManager::getInstance().createCompositeTexture({ texturePath[0], texturePath[1], texturePath[2], texturePath[3] }, { blendParams.first, blendParams.second });
        const uint32_t detailMapID = TextureManager::getInstance().loadTextureFromFile(texturePath[4]);
        meshData.material.baseColorTextureID = textureID;
        meshData.material.detailTextureID = detailMapID;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to load terrain multi texture: " << e.what() << std::endl;
    }
}
