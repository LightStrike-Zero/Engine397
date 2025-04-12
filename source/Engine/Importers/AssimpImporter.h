/**
 * @file AssimpImporter.h
 * @brief Concrete implementation for importing 3D models using the Assimp library.
 *
 * The AssimpImporter class is responsible for loading 3D models, processing their
 * mesh and material data, and normalizing their scale. It provides helper functions
 * to handle Assimp structures and convert embedded textures.
 *
 * @details
 * - `loadModel`: Loads a model from a file and extracts mesh data.
 * - `setupMesh`: Configures a RenderableComponent using the loaded mesh data.
 * - `processNode`: Recursively processes nodes in the Assimp scene graph.
 * - `processMesh`: Extracts mesh data from an Assimp mesh.
 * - `extractMaterialData`: Extracts material data from an Assimp material.
 * - `normalizeModelScale`: Normalizes the scale of the model to a target size.
 * - `convertEmbeddedTexture`: Converts embedded textures to raw image data.
 *
 * @author Shaun Matthews
 * @date Created: 29/11/2024.
 * @modified: 12/04/2025.
 */
#ifndef ASSIMPIMPORTER_H
#define ASSIMPIMPORTER_H

#include <string>
#include <assimp/scene.h>

#include "Components/DataTypes.h"
#include "Components/RenderableComponent.h"
#include "Texture/ImageData.h"

/**
 * @class AssimpImporter
 * @brief A concrete implementation for importing 3D models using the Assimp library.
 *
 * The AssimpImporter class provides methods to load models, process their
 * mesh and material data, and normalize their scale. It also includes helper
 * functions for handling Assimp structures and converting embedded textures.
 */
class AssimpImporter
{
public:
    /**
     * @brief Default constructor for AssimpImporter.
     */
    AssimpImporter() = default;

    /**
     * @brief Default destructor for AssimpImporter.
     */
    ~AssimpImporter() = default;

    /**
     * @brief Load a model file and populate mesh data.
     *
     * This method loads a 3D model from the specified file path and extracts
     * its mesh data into a vector of RawMeshData structures.
     *
     * @param filepath The path to the model file.
     * @param meshes A reference to a vector where the extracted mesh data will be stored.
     * @return True if the model was successfully loaded, false otherwise.
     */
    bool loadModel(const std::string& filepath, std::vector<RawMeshData>& meshes);

    /**
     * @brief Configure a RenderableComponent using loaded mesh data.
     *
     * This method sets up a RenderableComponent with the mesh data extracted
     * from a previously loaded model.
     *
     * @param renderableComponent The RenderableComponent to configure.
     */
    void setupMesh(RenderableComponent& renderableComponent);

private:
    /**
     * @brief Process a node in the Assimp scene graph.
     *
     * This method recursively processes a node in the Assimp scene graph,
     * extracting mesh data and applying transformations.
     *
     * @param node The Assimp node to process.
     * @param scene The Assimp scene containing the node.
     * @param parentTransform The transformation matrix of the parent node.
     * @param meshes A reference to a vector where the extracted mesh data will be stored.
     * @param filepath The path to the model file.
     */
    void processNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform,
                     std::vector<RawMeshData>& meshes,const std::string& filepath);

    /**
     * @brief Process an Assimp mesh and extract its data.
     *
     * This method extracts mesh data from an Assimp mesh and returns it as a RawMeshData structure.
     *
     * @param mesh The Assimp mesh to process.
     * @param scene The Assimp scene containing the mesh.
     * @param filepath The path to the model file.
     * @return A RawMeshData structure containing the extracted mesh data.
     */
    RawMeshData processMesh(aiMesh* mesh, const aiScene* scene, const std::string& filepath);

    /**
     * @brief Extract material data from an Assimp material.
     *
     * This method extracts material data from an Assimp material and returns it as a RawMaterialData structure.
     *
     * @param material The Assimp material to process.
     * @param scene The Assimp scene containing the material.
     * @param modelFilePath The path to the model file.
     * @return A RawMaterialData structure containing the extracted material data.
     */
    RawMaterialData extractMaterialData(aiMaterial* material, const aiScene* scene, const std::string& modelFilePath);

    /**
     * @brief Normalize the scale of the model to a target size.
     *
     * This method adjusts the scale of the model to ensure it fits within the specified target size.
     *
     * @param meshes A reference to a vector of RawMeshData structures representing the model's meshes.
     * @param targetSize The target size to normalize the model to.
     */
    void normalizeModelScale(std::vector<RawMeshData>& meshes, float targetSize);

    /**
     * @brief Convert an embedded texture to raw image data.
     *
     * This method converts an embedded texture from an Assimp model to a RawImageData structure.
     *
     * @param aiTex The Assimp texture to convert.
     * @return A RawImageData structure containing the converted texture data.
     */
    RawImageData convertEmbeddedTexture(const aiTexture* aiTex);
};
/**
 * @brief Generate a unique name for a material.
 *
 * This function generates a unique name for a material based on its properties.
 *
 * @param material The Assimp material to generate a name for.
 * @return A string containing the generated material name.
 */
std::string generateMaterialName(aiMaterial* material);

#endif //ASSIMPIMPORTER_H
