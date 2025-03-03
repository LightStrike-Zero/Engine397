//
// Created by Shaun on 1/12/2024.
//

#include "TextureLoader.h"

#include <iostream>

Texture* TextureLoader::loadTexture(const std::string& filePath) {
    // Create and return a new Texture object
    return new Texture(filePath);
}

Texture* TextureLoader::loadFromGLTF(const std::string& gltfTexturePath) {
    // Placeholder for GLTF-specific texture handling
    // In a real implementation, you would process GLTF files using Assimp or another library
    return new Texture(gltfTexturePath);
}

Texture* TextureLoader::loadEmbeddedTexture(aiTexture* embeddedTexture) {
    if (embeddedTexture->mHeight == 0) {
        // Compressed texture (e.g., PNG, JPEG)
        return new Texture(reinterpret_cast<unsigned char*>(embeddedTexture->pcData), embeddedTexture->mWidth);
    } else {
        // Uncompressed texture (e.g., BMP)
        // Handle uncompressed texture data
        std::cerr << "Uncompressed embedded textures are not supported yet." << std::endl;
        return nullptr;
    }
}