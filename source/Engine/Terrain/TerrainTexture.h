
#ifndef TERRAINTEXTURE_H
#define TERRAINTEXTURE_H

#include <memory>
#include <string>

#include "TerrainType.h"

class TerrainTexture : public TerrainType
{
public:
    //TerrainTexture(std::shared_ptr<TerrainType> wrappedTerrainType, const std::string& texturePath);
    TerrainTexture(std::shared_ptr<TerrainType> base, const std::string& texturePath, int repeatX, int repeatY);


    virtual void apply(RawMeshData& meshData, int numRows, int numCols) override;

private:
    std::shared_ptr<TerrainType> m_wrappedTerrainType;
    std::string m_texturePath;
    int m_repeatX = 1;
    int m_repeatY = 1;

    void loadTerrainTexture(RawMeshData& meshData, const std::string& texturePath);
};

#endif //TERRAINTEXTURE_H
