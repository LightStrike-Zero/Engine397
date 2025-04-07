
#ifndef TERRAINTEXTURE_H
#define TERRAINTEXTURE_H

#include <memory>
#include <string>

#include "TerrainType.h"

class TerrainTexture : public TerrainType
{
public:
    TerrainTexture(std::shared_ptr<TerrainType> wrappedTerrainType, const std::string& texturePath);
    virtual void apply(RawMeshData& meshData, int numRows, int numCols) override;

private:
    std::shared_ptr<TerrainType> m_wrappedTerrainType;
    std::string m_texturePath;

    void loadTerrainTexture(RawMeshData& meshData, const std::string& texturePath);
};

#endif //TERRAINTEXTURE_H
