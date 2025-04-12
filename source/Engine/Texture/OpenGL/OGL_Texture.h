/**
* @file OGL_Texture.h
 * @brief Defines the OpenGLTexture class for managing OpenGL textures.
 *
 * The OpenGLTexture class provides functionality for creating, binding, and managing
 * textures in an OpenGL context. It supports loading textures from files, memory, 
 * and raw image data.
 *
 * @details
 * - Implements the ITexture interface.
 * - Handles OpenGL-specific texture operations.
 * - Supports 2D textures by default.
 *
 * @date Created: 11/04/2025
 * @date Modified: 12/04/2025
 */

#ifndef OGL_TEXTURE_H
#define OGL_TEXTURE_H

#include "Interfaces/ITexture.h"
#include <string>
#include <GL/glew.h>

#include "Texture/ImageData.h"

/**
 * @class OpenGLTexture
 * @brief A final class for managing OpenGL textures.
 *
 * The OpenGLTexture class provides methods to load, bind, and retrieve texture
 * properties. It is designed specifically for OpenGL and assumes 2D textures by default.
 */
class OpenGLTexture final : public ITexture {
public:

    /**
     * @brief Default constructor for creating an empty OpenGL texture.
     */
    OpenGLTexture();

    /**
     * @brief Constructor to load a texture from a file.
     * @param filePath The path to the texture file.
     * @param textureType The OpenGL texture type (default is GL_TEXTURE_2D).
     */
    explicit OpenGLTexture(const std::string& filePath, GLenum textureType = GL_TEXTURE_2D);

    /**
    * @brief Constructor to load a texture from raw memory.
    * @param data Pointer to the raw texture data.
    * @param size The size of the texture data in bytes.
    * @param textureType The OpenGL texture type (default is GL_TEXTURE_2D).
    */
    explicit OpenGLTexture(unsigned char* data, size_t size, GLenum textureType = GL_TEXTURE_2D);

    /**
     * @brief Constructor to load a texture from raw image data.
     * @param imageData The raw image data structure.
     * @param textureType The OpenGL texture type (default is GL_TEXTURE_2D).
     */
    explicit OpenGLTexture(const RawImageData& imageData, GLenum textureType = GL_TEXTURE_2D);

    /**
     * @brief Destructor to clean up the OpenGL texture.
     */
    ~OpenGLTexture() override;

    /**
     * @brief Bind the texture to a specified slot.
     * @param slot The texture slot to bind to.
     */
    void bind(unsigned int slot) const override;

    /**
    * @brief Unbind the texture.
    */
    void unbind() const override;

    /**
     * @brief Get the width of the texture.
     * @return The width of the texture in pixels.
     */
    [[nodiscard]] uint32_t getWidth() const override { return m_width; }

    /**
     * @brief Get the height of the texture.
     * @return The height of the texture in pixels.
     */
    [[nodiscard]] uint32_t getHeight() const override { return m_height; }

    /**
     * @brief Get the number of channels in the texture.
     * @return The number of channels (e.g., 3 for RGB, 4 for RGBA).
     */
    [[nodiscard]] uint8_t getChannels() const override { return m_channels; }

    /**
     * @brief Get the OpenGL texture ID.
     * @return The OpenGL texture ID.
     */
    [[nodiscard]] GLuint getID() const override { return m_textureID; }

    /**
     * @brief Get the file path of the texture.
     * @return The file path as a string.
     */
    [[nodiscard]] const std::string& getPath() const { return m_filePath; }

    /**
     * @brief Load image data from a file.
     * @param filePath The path to the texture file.
     */
    void loadImageData(const std::string& filePath) override;
protected:
    /**
     * @brief Load texture data from a file.
     * @param filePath The path to the texture file.
     */
    void loadFromFile(const std::string& filePath);

    /**
     * @brief Load texture data from raw memory.
     * @param data Pointer to the raw texture data.
     * @param size The size of the texture data in bytes.
     */
    void loadFromMemory(unsigned char* data, size_t size);

    /**
     * @brief Load texture data from raw image data.
     * @param imageData The raw image data structure.
     */
    void loadFromRawData(const RawImageData& imageData);

public:
    /**
     * @brief Get the raw texture data.
     * @return A pointer to the raw texture data.
     */
    [[nodiscard]] const unsigned char* getData() const override { return m_data; };

private:
    GLuint m_textureID;     ///< The OpenGL texture ID.
    GLenum m_textureType;   ///< The OpenGL texture type (e.g., GL_TEXTURE_2D).
    std::string m_filePath; ///< The file path of the texture.
    unsigned char* m_data;  ///< Pointer to the raw texture data.
    uint8_t m_channels;     ///< The number of channels in the texture.
    uint32_t m_width;       ///< The width of the texture in pixels.
    uint32_t m_height;      ///< The height of the texture in pixels.
};

#endif //OGL_TEXTURE_H
