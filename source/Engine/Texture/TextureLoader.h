/**
 * @file TextureLoader.h
 * @brief Provides functionality for loading textures from various sources.
 *
 * The TextureLoader class offers static methods to load textures from file
 * paths, GLTF files, or raw image data in memory. It is designed to simplify
 * texture loading operations in the engine.
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

#include "ImageData.h"
#include "Interfaces/ICubeMap.h"

#include <memory>
#include <string>

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
  static std::shared_ptr<ITexture> loadTexture(const std::string &filePath);

  /**
   * @brief Load a texture from a GLTF file.
   *
   * @param gltfTexturePath The path to the GLTF texture file.
   * @return A shared pointer to the loaded texture.
   */
  static std::shared_ptr<ITexture>
  loadFromGLTF(const std::string &gltfTexturePath);

  /**
   * @brief Load a texture from raw image data in memory.
   *
   * @param imageData The raw image data to load the texture from.
   * @return A shared pointer to the loaded texture.
   */
  static std::shared_ptr<ITexture>
  loadFromMemory(const RawImageData &imageData);

  /**
   * @brief Load a cubemap texture from an array of six file paths.
   *
   * The order of file paths is expected to be in the following order:
   * +X, -X, +Y, -Y, +Z, -Z.
   *
   * @param faces An array of six file paths for the cubemap faces.
   * @return A shared pointer to the loaded cubemap.
   */
  static std::shared_ptr<ICubeMap>
  loadCubeMap(const std::array<std::string, 6> &faces);
};

#endif // TEXTURELOADER_H
