/**
* @file TextureFactory.h
 * @brief Factory class for creating texture objects.
 *
 * The TextureFactory class provides static methods to create instances of textures
 * from various sources, such as file paths, raw memory, or raw image data.
 *
 * @details
 * - `createTexture()`: Creates an empty texture.
 * - `createTexture(const std::string& filePath)`: Creates a texture from a file.
 * - `createTexture(unsigned char* data, size_t size)`: Creates a texture from raw memory.
 * - `createTexture(const RawImageData& imageData)`: Creates a texture from raw image data.
 *
 * @author Shaun Matthews
 * @date Created: 12/04/2025
 */

#ifndef TEXTUREFACTORY_H
#define TEXTUREFACTORY_H
#include "Interfaces/ICubeMap.h"

#include <memory>
#include <string>

#include "Interfaces/ITexture.h"
#include "Texture/ImageData.h"


/**
 * @class TextureFactory
 * @brief A factory class for creating texture objects.
 *
 * The TextureFactory class provides static methods to create textures
 * from different sources, simplifying the process of texture creation.
 */
class TextureFactory {
public:
    /**
     * @brief Create an empty texture.
     * @return A shared pointer to the created texture.
     */
    static std::shared_ptr<ITexture> createTexture();

    /**
     * @brief Create a texture from a file.
     * @param filePath The path to the texture file.
     * @return A shared pointer to the created texture.
     */
    static std::shared_ptr<ITexture> createTexture(const std::string& filePath);

    /**
     * @brief Create a texture from raw memory.
     * @param data Pointer to the raw texture data.
     * @param size The size of the texture data in bytes.
     * @return A shared pointer to the created texture.
     */
    static std::shared_ptr<ITexture> createTexture(unsigned char* data, size_t size);

    /**
     * @brief Create a texture from raw image data.
     * @param imageData The raw image data structure.
     * @return A shared pointer to the created texture.
     */
    static std::shared_ptr<ITexture> createTexture(const RawImageData& imageData);

     /**
     * @brief Create a cubemap texture from an array of six file paths.
     *
     * The order of the file paths must match the following order
     * +X, -X, +Y, -Y, +Z, -Z.
     *
     * @param faces An array of six texture file paths.
     * @return A shared pointer to the created cubemap texture.
     */
     static std::shared_ptr<ICubeMap> createTexture(const std::array<std::string, 6>& faces);
};
#endif //TEXTUREFACTORY_H
