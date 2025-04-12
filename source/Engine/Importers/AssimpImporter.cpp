#include "AssimpImporter.h"

#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <Texture/TextureManager.h>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGL/OpenGLMeshBuffer.h"

bool AssimpImporter::loadModel(const std::string& filepath,
                               std::vector<RawMeshData>& meshes/*, std::vector<RawMaterialData>& materials*/)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath,
                                             aiProcess_Triangulate | // Ensure all faces are triangles
                                             aiProcess_FlipUVs |
                                             // Flip UVs to match the OpenGL texture coordinate space
                                             aiProcess_CalcTangentSpace | // Calculate tangents for normal mapping
                                             aiProcess_GenNormals | // Generate normals if not present
                                             aiProcess_JoinIdenticalVertices); // Remove redundant vertices

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return false;
    }

    // Since this is a recursive function, we supply the identity matrix as the parent transform
    // successive calls will multiply the parent transform with the current node's transform
    // in other words - each child mesh will transform relative to its parent
    glm::mat4 identity = glm::mat4(1.0f);
    processNode(scene->mRootNode, scene, identity, meshes, /*materials, */filepath);
    // NOTE: Materials are not being used here anymore, remove.

    return true;
}

void AssimpImporter::processNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform,
                                 std::vector<RawMeshData>& meshes, /*std::vector<RawMaterialData>& materials, */
                                 const std::string& filepath)
{
    glm::mat4 nodeTransform = glm::transpose(glm::make_mat4(&node->mTransformation.a1));
    glm::mat4 globalTransform = parentTransform * nodeTransform;

    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        RawMeshData meshData = processMesh(mesh, scene, filepath);

        // Store the node's global transform separately
        meshData.transform = globalTransform;
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            // std::cout << "Mesh Material Index: " << mesh->mMaterialIndex << std::endl;
            meshData.material = extractMaterialData(material, scene, filepath);
        }
        meshes.push_back(meshData);

        // Extract the material associated with this mesh
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, globalTransform, meshes/*, materials*/, filepath);
    }
}

RawMeshData AssimpImporter::processMesh(aiMesh* mesh, const aiScene* scene, const std::string& filepath)
{
    RawMeshData meshData;

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

        // process the texture coordinates
        if (mesh->mTextureCoords[0])
        {
            vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }
        // process the tangents
        if (mesh->mTangents)
        {
            vertex.tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
        }
        // process the bitangents
        if (mesh->mBitangents)
        {
            vertex.bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
        }

        meshData.vertices.push_back(vertex);
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            meshData.indices.push_back(face.mIndices[j]);
        }
    }

    return meshData; // No material is attached here.
}

void AssimpImporter::setupMesh(RenderableComponent& renderableComponent)
{
    renderableComponent.meshBuffer = std::make_shared<OpenGLMeshBuffer>(renderableComponent.indices.size(),
                                                                        renderableComponent.vertices.size(),
                                                                        renderableComponent.indices.data(),
                                                                        renderableComponent.vertices.data());
}


RawMaterialData AssimpImporter::extractMaterialData(aiMaterial* material,
                                                    const aiScene* scene,
                                                    const std::string& modelFilePath)
{
    RawMaterialData materialData;

    // Lambda that loads a texture either from an embedded source or from an external file.
    // It assigns the GPU texture ID to textureID.
    auto loadTexture = [&](aiTextureType type, uint32_t& textureID)
    {
        aiString path;
        if (material->GetTexture(type, 0, &path) == AI_SUCCESS)
        {
            std::string texturePath = path.C_Str();
            if (!texturePath.empty())
            {
                // Flag decal textures if the path contains "decal"
                materialData.isDecal = (texturePath.find("decal") != std::string::npos);

                if (texturePath[0] == '*')
                {
                    // Handle embedded texture
                    int textureIndex = atoi(texturePath.c_str() + 1);
                    if (textureIndex >= 0 && textureIndex < scene->mNumTextures)
                    {
                        aiTexture* embeddedTexture = scene->mTextures[textureIndex];
                        // Convert the embedded texture to our engine's RawImageData format.
                        RawImageData imageData = convertEmbeddedTexture(embeddedTexture);
                        // Load the texture from raw data and return its GPU texture ID.
                        textureID = TextureManager::getInstance().loadTextureFromData(imageData);
                    }
                }
                else
                {
                    // Handle external texture
                    std::string directory = modelFilePath.substr(0, modelFilePath.find_last_of("/\\"));
                    std::string resolvedPath = directory + "/" + texturePath;
                    // Load the texture from file and return the GPU texture ID.
                    textureID = TextureManager::getInstance().loadTextureFromFile(resolvedPath);
                }
            }
        }
    };

    // Load the individual textures for the material.
    loadTexture(aiTextureType_BASE_COLOR, materialData.baseColorTextureID);
    // Uncomment and use if you need a metalness texture:
    // loadTexture(aiTextureType_METALNESS, materialData.metalnessTextureID);
    loadTexture(aiTextureType_DIFFUSE_ROUGHNESS, materialData.roughnessTextureID);
    loadTexture(aiTextureType_NORMALS, materialData.normalTextureID);

    return materialData;
}

void AssimpImporter::normalizeModelScale(std::vector<RawMeshData>& meshes, float targetSize)
{
    glm::vec3 minBounds(std::numeric_limits<float>::max());
    glm::vec3 maxBounds(std::numeric_limits<float>::lowest());

    for (const auto& mesh : meshes)
    {
        for (const auto& vertex : mesh.vertices)
        {
            minBounds = glm::min(minBounds, vertex.position);
            maxBounds = glm::max(maxBounds, vertex.position);
        }
    }

    glm::vec3 dimensions = maxBounds - minBounds;
    float maxDimension = std::max(dimensions.x, std::max(dimensions.y, dimensions.z));
    float scale = targetSize / maxDimension;

    for (auto& mesh : meshes)
    {
        for (auto& vertex : mesh.vertices)
        {
            vertex.position *= scale;
        }
    }
}

RawImageData AssimpImporter::convertEmbeddedTexture(const aiTexture* aiTex)
{
    if (!aiTex)
    {
        throw std::runtime_error("Embedded texture pointer is null.");
    }

    // this is checking if the texture is compressed or not
    // Compressed Texture
    if (aiTex->mHeight == 0)
    {
        auto dataSize = static_cast<size_t>(aiTex->mWidth);

        unsigned char* dataCopy = new unsigned char[dataSize];
        memcpy(dataCopy, aiTex->pcData, dataSize);

        std::string format = aiTex->achFormatHint[0] != '\0' ? std::string(aiTex->achFormatHint) : "Unknown";
        return {dataCopy, dataSize, format};
    }
    // Uncompressed Texture
    else
    {
        int width = static_cast<int>(aiTex->mWidth);
        int height = static_cast<int>(aiTex->mHeight);

        const int channels = 4; // Assuming RGBA format currently but need to revisit in future
        size_t dataSize = width * height * channels;
        auto* dataCopy = new unsigned char[dataSize];
        memcpy(dataCopy, aiTex->pcData, dataSize);

        return {dataCopy, width, height, dataSize};
    }
}

std::string generateMaterialName(aiMaterial* material)
{
    aiString texturePath;
    if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
    {
        return "Material_" + std::string(texturePath.C_Str());
    }
    return "Material_Default";
}
