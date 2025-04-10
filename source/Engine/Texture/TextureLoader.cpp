//
// Created by Shaun on 1/12/2024.
//

#include "TextureLoader.h"

#include <iostream>

std::shared_ptr<ITexture> TextureLoader::loadTexture(const std::string& filePath) {
    return new Texture(filePath);
}

std::shared_ptr<ITexture> TextureLoader::loadFromGLTF(const std::string& gltfTexturePath) {
   
    return new Texture(gltfTexturePath);
}

std::shared_ptr<ITexture> TextureLoader::loadEmbeddedTexture(void* embeddedTexture) {
    if (embeddedTexture->mHeight == 0) {
        return new Texture(reinterpret_cast<unsigned char*>(embeddedTexture->pcData), embeddedTexture->mWidth);
    } else {
       
        std::cerr << "Uncompressed embedded textures are not supported yet." << std::endl;
        return nullptr;
    }
}