//
// Created by Shaun on 29/11/2024.
//

#ifndef ASSIMPIMPORTER_H
#define ASSIMPIMPORTER_H

#include <string>
#include <assimp/scene.h>

#include "MaterialData.h"
#include "Components/DataTypes.h"
#include "Components/RenderableComponent.h"




class AssimpImporter {
public:
    AssimpImporter() = default;
    ~AssimpImporter() = default;

    // Load a model file and populate ModelData
    bool loadModel(const std::string& filepath, std::vector<RawMeshData>& meshes/*, std::vector<RawMaterialData>& materials*/);

    void setupMesh(RenderableComponent& renderableComponent);
private:
    // Helper functions to process Assimp structures
    void processNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform, std::vector<RawMeshData>& meshes/*, std::vector<RawMaterialData>& materials*/, const std::string& filepath);
    RawMeshData processMesh(aiMesh* mesh, const aiScene* scene, const std::string& filepath);
    // RawMaterialData processMaterial(aiMaterial* material, const aiScene* scene, const std::string& modelFilePath);
    RawMaterialData extractMaterialData(aiMaterial* material, const aiScene* scene, const std::string& modelFilePath);
    void normalizeModelScale(std::vector<RawMeshData>& meshes, float targetSize);

};

std::string generateMaterialName(aiMaterial* material);

#endif //ASSIMPIMPORTER_H
