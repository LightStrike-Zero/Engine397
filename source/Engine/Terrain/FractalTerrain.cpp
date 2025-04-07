
#include "FractalTerrain.h"

#include <random>
#include <glm/ext/scalar_constants.hpp>

FractalTerrain::FractalTerrain(int iterations, float initialDisplacement, float displacementDecay, float heightScale, unsigned int seed, float smoothness, int smoothingPasses)
    : m_iterations(iterations),
      m_initialDisplacement(initialDisplacement),
      m_displacementDecay(displacementDecay),
      m_heightScale(heightScale),
        m_seed(seed),
        m_kernalStrength(smoothness),
        m_smoothingPasses(smoothingPasses)
{
}

void FractalTerrain::apply(RawMeshData& meshData, const int numRows, const int numCols)
{
    generateFractalHeightmap(numRows, numCols, m_iterations, m_initialDisplacement, m_displacementDecay, m_seed, m_kernalStrength, m_smoothingPasses);
    
    for (auto& vertex : meshData.vertices)
    {
        float u = vertex.texCoords.x;
        float v = vertex.texCoords.y;
        
        int x = static_cast<int>(u * (numCols - 1));
        int y = static_cast<int>(v * (numRows - 1));
        
        x = std::min(std::max(x, 0), numCols - 1);
        y = std::min(std::max(y, 0), numRows - 1);
        
        int index = y * numCols + x;
        float height = static_cast<float>(m_generatedHeightMap[index]) / 255.0f;
        
        vertex.position.y = height * m_heightScale;
    }
}

void FractalTerrain::generateFractalHeightmap(int numRows, int numCols, int iterations, float initialDisplacement, float displacementDecay, unsigned int seed, float smoothness, int smoothingPasses)
{
    std::vector<float> heightMap(numRows * numCols, 0.0f);

    std::default_random_engine engine(seed);
    std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.1415F);
    std::uniform_int_distribution<int> colDist(0, numCols - 1);
    std::uniform_int_distribution<int> rowDist(0, numRows - 1);

    float displacement = initialDisplacement;

    for (int i = 0; i < iterations; ++i)
    {
        float angle = angleDist(engine);
        float a = std::cos(angle);
        float b = std::sin(angle);

        int randomCol = colDist(engine);
        int randomRow = rowDist(engine);
        float d = a * randomCol + b * randomRow;

        for (int row = 0; row < numRows; ++row)
        {
            for (int col = 0; col < numCols; ++col)
            {
                float value = a * col + b * row - d;
                if (value > 0)
                    heightMap[row * numCols + col] += displacement;
                else
                    heightMap[row * numCols + col] -= displacement;
            }
        }
        displacement *= displacementDecay;
    }

    float minVal = *std::min_element(heightMap.begin(), heightMap.end());
    float maxVal = *std::max_element(heightMap.begin(), heightMap.end());
    float range = maxVal - minVal;

    m_generatedHeightMap.resize(numRows * numCols);
    for (size_t i = 0; i < heightMap.size(); i++)
    {
        m_generatedHeightMap[i] = (range > 0) ?
            static_cast<unsigned char>(255.0f * (heightMap[i] - minVal) / range) :
            0;
    }

    applyFIRFilter(numRows, numCols, smoothness, smoothingPasses);
}
void FractalTerrain::applyFIRFilter(int numRows, int numCols, float filterStrength, int passes)
{
    if (filterStrength < 0.0f)
        filterStrength = 0.0f;
    if (filterStrength > 1.0f)
        filterStrength = 1.0f;

    // 5x5 Gaussian kernel
    const float kernel[5][5] = {
        {1.0f, 4.0f, 6.0f, 4.0f, 1.0f},
        {4.0f, 16.0f, 24.0f, 16.0f, 4.0f},
        {6.0f, 24.0f, 36.0f, 24.0f, 6.0f},
        {4.0f, 16.0f, 24.0f, 16.0f, 4.0f},
        {1.0f, 4.0f, 6.0f, 4.0f, 1.0f}
    };
    const float kernelSum = 256.0f; 

    std::vector<unsigned char> smoothedHeightMap = m_generatedHeightMap;

    for (int pass = 0; pass < passes; ++pass) {
        std::vector<unsigned char> temp = smoothedHeightMap;
        for (int z = 2; z < numRows - 2; z++) {
            for (int x = 2; x < numCols - 2; x++) {
                float sum = 0.0f;
                for (int kz = -2; kz <= 2; kz++) {
                    for (int kx = -2; kx <= 2; kx++) {
                        int idx = (z + kz) * numCols + (x + kx);
                        sum += temp[idx] * kernel[kz + 2][kx + 2];
                    }
                }
                float filteredValue = sum / kernelSum;
                float originalValue = temp[z * numCols + x];
                float blendedValue = filterStrength * filteredValue + (1.0f - filterStrength) * originalValue;
                smoothedHeightMap[z * numCols + x] = static_cast<unsigned char>(blendedValue);
            }
        }
    }

    m_generatedHeightMap = smoothedHeightMap;
}