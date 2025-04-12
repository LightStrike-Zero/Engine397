/**
* @file TextureLoader.h
 * @brief Provides functionality for loading textures from various sources.
 *
 * The TextureLoader class offers static methods to load textures from file paths,
 * GLTF files, or raw image data in memory. It is designed to simplify texture
 * loading operations in the engine.
 *
 * @details
 * - `loadTexture`: Loads a texture from a file path.
 * - `loadFromGLTF`: Loads a texture from a GLTF file.
 * - `loadFromMemory`: Loads a texture from raw image data in memory.
 *
 * @date Created: 1/12/2024
 * @date Modified: 12/04/2025
 */

#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <string>
#include <memory>
#include "ImageData.h"

class ITexture;

/**
 * @class TextureLoader
 * @brief A utility class for loading textures from various sources.
 *
 * The TextureLoader class provides static methods to load textures
 * from file paths, GLTF files, or raw image data in memory.
 */
class TextureLoader {
public:
    /**
     * @brief Load a texture from a file.
     *
     * @param filePath The path to the texture file.
     * @return A shared pointer to the loaded texture.
     */
    static std::shared_ptr<ITexture> loadTexture(const std::string& filePath);

    /**
     * @brief Load a texture from a GLTF file.
     *
     * @param gltfTexturePath The path to the GLTF texture file.
     * @return A shared pointer to the loaded texture.
     */
    static std::shared_ptr<ITexture> loadFromGLTF(const std::string& gltfTexturePath);

    /**
     * @brief Load a texture from raw image data in memory.
     *
     * @param imageData The raw image data to load the texture from.
     * @return A shared pointer to the loaded texture.
     */
    static std::shared_ptr<ITexture> loadFromMemory(const RawImageData& imageData);
};

#endif //TEXTURELOADER_H
