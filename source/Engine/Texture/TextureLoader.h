//
// Created by Shaun on 1/12/2024.
//

/**
 * @file TextureLoader.h
 * @brief Utility class for loading textures from various sources (file, GLTF, embedded).
 *
 * Provides static functions for texture loading used throughout the engine.
 * Delegates actual OpenGL texture object creation to the Texture class.
 *
 * @author Shaun
 * @date Created: 1/12/2024
 */

#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <string>
#include <assimp/texture.h>

#include "Texture.h"


/**
 * @class TextureLoader
 * @brief Utility class for loading textures from various sources (file, GLTF, embedded).
 * Provides static functions for texture loading used throughout the engine.
 * Delegates actual OpenGL texture object creation to the Texture class.
 */
class TextureLoader {

public:
    /**
     * @brief Loads a standard texture from a file path.
     * @param filePath Path to the image file.
     * @return Pointer to the created Texture object.
     */
    static Texture* loadTexture(const std::string& filePath);

    /**
     * @brief Loads a texture from a GLTF source path.
     * @param gltfTexturePath Path from GLTF material.
     * @return Pointer to the created Texture object.
     */
    static Texture* loadFromGLTF(const std::string& gltfTexturePath);

    /**
     * @brief Loads an embedded texture from Assimp.
     * @param embeddedTexture Pointer to the embedded aiTexture.
     * @return Pointer to the created Texture object.
     */
    static Texture* loadEmbeddedTexture(aiTexture* embeddedTexture);
};

#endif //TEXTURELOADER_H
