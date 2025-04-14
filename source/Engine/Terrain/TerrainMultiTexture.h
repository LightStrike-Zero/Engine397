/**
 * @file TerrainTexture.h
 * @brief Defines the TerrainTexture class that applies textures to terrain.
 * @author Shaun
 * @date 13/04/2025.
 */

#ifndef TERRAINMULTITEXTURE_H
#define TERRAINMULTITEXTURE_H

#include <memory>
#include <string>

#include "TerrainType.h"

/**
 * @class TerrainMultiTexture
 * @brief Decorator class that adds multi-texturing to an existing TerrainType.
 */
class TerrainMultiTexture : public TerrainType
{
public:
    /**
     * @brief Constructs a multi-textured terrain by wrapping a base terrain.
     * @param base Base terrain to wrap.
     * @param texturePath File path to one of the texture images. 
     *        (You can extend this to a vector if you need multiple paths.)
     * @param repeatX Number of horizontal texture repetitions.
     * @param repeatY Number of vertical texture repetitions.
     */
    TerrainMultiTexture(std::shared_ptr<TerrainType> base, const std::string& texturePath, int repeatX, int repeatY);

    /**
     * @copydoc TerrainType::apply
     */
    virtual void apply(RawMeshData& meshData, int numRows, int numCols) override;

private:
    std::shared_ptr<TerrainType> m_wrappedTerrainType;
    std::string m_texturePath;
    int m_repeatX = 1;
    int m_repeatY = 1;

    /**
     * @brief Loads and applies multi-texture blending to the mesh.
     * @param meshData Reference to mesh data that will receive the texture.
     * @param texturePath Path to the base texture. (Extendable to include additional textures.)
     */
    void loadTerrainMultiTexture(RawMeshData& meshData, const std::string& texturePath);
};

#endif //TERRAINMULTITEXTURE_H
