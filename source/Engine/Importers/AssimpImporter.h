//
// Created by Shaun on 29/11/2024.
//

/**
 * @file AssimpImporter.h
 * @brief Handles the import of 3D models using Assimp and processes them into engine-compatible formats.
 * @author Shaun
 * @date 2024-11-29
 */

#ifndef ASSIMPIMPORTER_H
#define ASSIMPIMPORTER_H

#include <string>
#include <assimp/scene.h>

#include "Components/DataTypes.h"
#include "Components/RenderableComponent.h"

/**
 * @class AssimpImporter
 * @brief Imports 3D models using Assimp and converts them into raw mesh/material data for use in the engine.
 */
class AssimpImporter {
public:
    AssimpImporter() = default;
    ~AssimpImporter() = default;


    /**
     * @brief Loads a model from the specified file path nd populate ModelData.
     * @param filepath The file path to the model.
     * @param meshes The output vector that will hold raw mesh data.
     * @return true if loading is successful, false otherwise.
     */
    bool loadModel(const std::string& filepath, std::vector<RawMeshData>& meshes/*, std::vector<RawMaterialData>& materials*/);

    /**
     * @brief Sets up the mesh for rendering by populating the renderable component.
     * @param renderableComponent The component to populate.
     */
    void setupMesh(RenderableComponent& renderableComponent);

private:
    // Helper functions to process Assimp structures
    void processNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform, std::vector<RawMeshData>& meshes/*, std::vector<RawMaterialData>& materials*/, const std::string& filepath);
    RawMeshData processMesh(aiMesh* mesh, const aiScene* scene, const std::string& filepath);
    // RawMaterialData processMaterial(aiMaterial* material, const aiScene* scene, const std::string& modelFilePath);
    RawMaterialData extractMaterialData(aiMaterial* material, const aiScene* scene, const std::string& modelFilePath);
    void normalizeModelScale(std::vector<RawMeshData>& meshes, float targetSize);

};

/**
 * @brief Generates a name string from an Assimp material.
 * @param material Pointer to the Assimp material.
 * @return A string identifier for the material.
 */
std::string generateMaterialName(aiMaterial* material);

#endif //ASSIMPIMPORTER_H
