//
// Created by Shaun on 1/12/2024.
//

#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <string>
#include <assimp/texture.h>

#include "Texture.h"

class TextureLoader {
public:
    static Texture* loadTexture(const std::string& filePath);
    static Texture* loadFromGLTF(const std::string& gltfTexturePath);
    static Texture* loadEmbeddedTexture(aiTexture* embeddedTexture);
};

#endif //TEXTURELOADER_H
