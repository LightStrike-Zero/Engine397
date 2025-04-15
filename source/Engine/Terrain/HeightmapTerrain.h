/**
 * @file HeightmapTerrain.h
 * @brief Defines the HeightmapTerrain class for generating terrain from heightmaps.
 * @author Shaun
 * @date March 2025
 */

#ifndef UNTEXTUREDTERRAIN_H
#define UNTEXTUREDTERRAIN_H
#include <string>

#include "TerrainType.h"

/**
 * @class HeightmapTerrain
 * @brief Implements a terrain type based on heightmap images.
 */
class HeightmapTerrain : public TerrainType {
public:

    /**
     * @brief Constructor that loads a heightmap image.
     * @param heightMapPath Path to the heightmap image.
     * @param heightScale Scale applied to the height values.
     */
    explicit HeightmapTerrain(const std::string& heightMapPath, float heightScale = 1.0f);

    /**
     * @copydoc TerrainType::apply
     */
    virtual void apply(RawMeshData& meshData, const int numRows, const int numCols) override;

protected:
    std::string m_heightmapPath;                           ///< Path to the heightmap file.
    float m_heightScale;                                   ///< Vertical scale factor for heightmap values.
    const unsigned char* m_heightMapData = nullptr;        ///< Raw image data.
    int m_heightMapWidth;                                  ///< Width of the heightmap image.
    int m_heightMapHeight;                                 ///< Height of the heightmap image.
    int m_heightMapChannels;                               ///< Number of channels in the heightmap image.

    /**
     * @brief Loads heightmap data from a file.
     * @param heightMapPath Path to the heightmap.
     */
    void loadHeightMap(const std::string& heightMapPath);

    /**
     * @brief Converts a pixel value to a float height.
     */
    float getPixelAsFloat(float x, float y);

    /**
     * @brief Samples heightmap using bilinear filtering.
     */
    float bilinearSample(float xf, float yf);
    /**
     * @brief Computes the gradient normals for the mesh.
     * @param meshData Reference to the mesh data.
     * @param numRows Number of rows in the mesh.
     * @param numCols Number of columns in the mesh.
     */
    void computeMeshGradientNormals(RawMeshData& meshData, int numRows, int numCols);

    /*
     * @brief Smooths the heightmap data using a simple averaging filter.
     */
    void smoothHeightmapData();

    /*
     * brief Smooths the vertex positions of the mesh using a Gaussian filter.
     */
    void smoothVertexPositions(RawMeshData& meshData, int numRows, int numCols);
};

#endif //UNTEXTUREDTERRAIN_H
