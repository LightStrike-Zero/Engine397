/**
 * @file TerrainTexture.h
 * @brief Defines the TerrainTexture class that applies textures to terrain.
 * @author Shaun with minor changes by Buko
 * @date March 2025
 */

#ifndef TERRAINTEXTURE_H
#define TERRAINTEXTURE_H

#include <memory>
#include <string>

#include "TerrainType.h"

/**
 * @class TerrainTexture
 * @brief Decorator class that adds texture mapping to an existing TerrainType.
 */
class TerrainTexture : public TerrainType
{
public:
    //TerrainTexture(std::shared_ptr<TerrainType> wrappedTerrainType, const std::string& texturePath);

    /**
     * @brief Constructor that wraps an existing terrain type and applies a texture.
     * @param base Base terrain to wrap.
     * @param texturePath File path to the texture image.
     * @param repeatX Number of horizontal repetitions of the texture.
     * @param repeatY Number of vertical repetitions of the texture.
     */
    TerrainTexture(std::shared_ptr<TerrainType> base, const std::string& texturePath, int repeatX, int repeatY);

    /**
     * @copydoc TerrainType::apply
     */
    virtual void apply(RawMeshData& meshData, int numRows, int numCols) override;

private:
    std::shared_ptr<TerrainType> m_wrappedTerrainType;
    std::string m_texturePath;
    int m_repeatX = 1;
    int m_repeatY = 1;

    void loadTerrainTexture(RawMeshData& meshData, const std::string& texturePath);
};

#endif //TERRAINTEXTURE_H
