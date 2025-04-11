/**
 * @file FractalTerrain.h
 * @brief Defines the FractalTerrain class for generating terrain using a fault formation algorithm with optional smoothing.
 * @author Shaun
 * @date March 2025
 */

#ifndef FRACTALTERRAIN_H
#define FRACTALTERRAIN_H

#include <random>
#include <vector>

#include "TerrainType.h"

/**
 * @class FractalTerrain
 * @brief Implements a terrain generation method using fault formation (fractal) algorithm.
 *
 * Generates heightmaps procedurally by simulating random fault lines and displacing terrain,
 * with optional smoothing via a Gaussian FIR filter.
 */
class FractalTerrain : public TerrainType {

public:

    /**
     * @brief Constructs a FractalTerrain generator with the given parameters.
     *
     * @param iterations Number of fault iterations to apply.
     * @param initialDisplacement Initial height displacement magnitude.
     * @param displacementDecay Factor by which displacement reduces per iteration.
     * @param heightScale Multiplier for final height values applied to terrain.
     * @param seed Random seed for repeatability.
     * @param smoothness Filter blending strength between 0.0 and 1.0.
     * @param smoothingPasses Number of FIR filter passes to apply.
     */
    FractalTerrain(int iterations, float initialDisplacement, float displacementDecay, float heightScale, unsigned int seed, float smoothness, int smoothingPasses);

    /**
     * @copydoc TerrainType::apply
     */
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
