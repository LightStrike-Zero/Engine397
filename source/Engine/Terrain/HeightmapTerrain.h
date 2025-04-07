
#ifndef UNTEXTUREDTERRAIN_H
#define UNTEXTUREDTERRAIN_H
#include <string>

#include "TerrainType.h"

class HeightmapTerrain : public TerrainType {
public:
    explicit HeightmapTerrain(const std::string& heightMapPath, float heightScale = 1.0f);

    virtual void apply(RawMeshData& meshData, const int numRows, const int numCols) override;

protected:
    std::string m_heightmapPath;
    float m_heightScale;
    const unsigned char* m_heightMapData = nullptr;
    int m_heightMapWidth;
    int m_heightMapHeight;
    int m_heightMapChannels;

    void loadHeightMap(const std::string& heightMapPath);
    float getPixelAsFloat(float x, float y);
    float bilinearSample(float xf, float yf);
};

#endif //UNTEXTUREDTERRAIN_H
