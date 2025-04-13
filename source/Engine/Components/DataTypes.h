//
// Created by Shaun on 6/04/2025.
//

/**
 * @file DataTypes.h
 * @brief Defines several DataType structs: Material Data, RawMaterialData, Vertex, RawMeshData .
 * @author Shaun
 * @date 2025-04-06
 */

#ifndef DATATYPES_H
#define DATATYPES_H
#include <vector>
#include <glm/glm.hpp>

class Texture;

/**
 * @struct MaterialData
 * @brief Stores classic lighting model parameters and optional texture IDs.
 */
struct MaterialData {
    glm::vec3 diffuseColor;         ///< Diffuse surface colour
    glm::vec3 specularColor;        ///< Specular highlight colour
    glm::vec3 ambientColor;         ///< Ambient light colour
    float shininess = 32.0f;        ///< Shininess coefficient for specular reflection

    uint8_t diffuseTextureID = 0;    ///< OpenGL Texture ID for the diffuseColor texture
    uint8_t specularTextureID = 0;   ///< OpenGL Texture ID for the specularColor texture

    bool isDecal = false;           ///< Whether this material should be rendered as a decal

    MaterialData() = default;

    /**
     * @brief Parameterised constructor.
     * @param diffuse Diffuse colour
     * @param specular Specular colour
     * @param shininess Shininess value
     */
    MaterialData(const glm::vec3& diffuse, const glm::vec3& specular, float shininess)
        : diffuseColor(diffuse), specularColor(specular), shininess(shininess) {}

};

/**
 * @struct RawMaterialData
 * @brief Contains texture IDs for PBR materials and optional decal flag.
 */
struct RawMaterialData {
    uint_fast32_t baseColorTextureID = 0;      ///< Albedo (base colour) texture
    uint_fast32_t metalnessTextureID = 0;      ///< Metalness texture
    uint_fast32_t roughnessTextureID = 0;      ///< Roughness texture
    uint_fast32_t normalTextureID = 0;         ///< Normal map texture
    bool isDecal = false;               ///< Decal flag
};

/**
 * @struct Vertex
 * @brief Stores a single point/vertex.
 */
struct Vertex {
    glm::vec3 position;             ///< Position in world space
    glm::vec2 texCoords;            ///< Texture coordinates
    glm::vec3 normal;               ///< Normal vector
    glm::vec3 tangent;              ///< Tangent vector for normal mapping
    glm::vec3 bitangent;            ///< Bitangent vector for normal mapping
};

/**
 * @struct RawMeshData
 * @brief A raw mesh data containing geometry (vertices and indices), material, and transform.
 */
struct RawMeshData {
    std::vector<Vertex> vertices;                ///< List of mesh vertices
    std::vector<unsigned int> indices;           ///< List of indices for indexed rendering
    RawMaterialData material;                    ///< Material applied to the mesh
    glm::mat4 transform = glm::mat4(1.0f);    ///< Global transformation matrix of the mesh
};


#endif //DATATYPES_H
