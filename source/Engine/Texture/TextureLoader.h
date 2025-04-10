//
// Created by Shaun on 1/12/2024.
//

#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <string>
#include <memory>

class ITexture;

class TextureLoader {
public:
    static std::shared_ptr<ITexture> loadTexture(const std::string& filePath);
    static std::shared_ptr<ITexture> loadFromGLTF(const std::string& gltfTexturePath);
    static std::shared_ptr<ITexture> loadEmbeddedTexture(void* embeddedTexture);
};

#endif //TEXTURELOADER_H
