
#ifndef FRACTALTERRAIN_H
#define FRACTALTERRAIN_H

#include <random>
#include <vector>

#include "TerrainType.h"


class FractalTerrain : public TerrainType {

public:

    FractalTerrain(int iterations, float initialDisplacement, float displacementDecay, float heightScale, unsigned int seed, float smoothness, int smoothingPasses);

    virtual void apply(RawMeshData& meshData, const int numRows, const int numCols) override;

private:
    int m_iterations;
    float m_initialDisplacement;
    float m_displacementDecay;
    float m_heightScale;
    unsigned int m_seed;
    float m_kernalStrength;
    int m_smoothingPasses;
    std::vector<unsigned char> m_generatedHeightMap;

    void generateFractalHeightmap(int numRows, int numCols, int iterations, float initialDisplacement, float displacementDecay, unsigned int seed, float smoothness, int smoothingPasses);
    void applyFIRFilter(int numRows, int numCols, float filterStrength, int passes = 1);
};



#endif //FRACTALTERRAIN_H
