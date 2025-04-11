//
// Created by Shaun on 06/10/2024.
//

/**
 * @file Texture.h
 * @brief Provides loading and binding functionality for OpenGL textures.
 *
 * Handles standard 2D textures for rendering, including image data loading via stb_image.
 *
 * @author Shaun
 * @date Created: 2024-10-06
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>


/**
 * @class Texture
 * @brief Provides loading and binding functionality for OpenGL textureS.
 * Handles standard 2D textures for rendering, including image data loading via stb_image.
 */
class Texture {
public:

    /**
     * @brief Default constructor.
     */
    Texture();
    // TODO these should be virtual, and not contain any OpenGL code
    explicit Texture(const std::string& filePath, GLenum textureType = GL_TEXTURE_2D);
    explicit Texture(unsigned char* data, size_t size, GLenum textureType = GL_TEXTURE_2D);

    /**
     * @brief Destructor. Frees OpenGL texture memory.
     */
    ~Texture();
    void loadImageData(const std::string& filePath);

    /**
     * @brief Gets the raw pixel data.
     * @return Pointer to the texture image data.
     */
    const unsigned char* getData() const { return m_data; }

    /**
     * @brief Gets the width of the texture.
     * @return Width in pixels.
     */
    int getWidth() const { return m_width; }

    /**
     * @brief Gets the height of the texture.
     * @return Height in pixels.
     */
    int getHeight() const { return m_height; }

    /**
     * @brief Gets the number of colour channels.
     * @return Number of channels (e.g., 3 for RGB).
     */
    int getChannels() const { return m_channels; }

    // TODO these should be virtual, then we can have a OpenGLTexture class that inherits from this
    /**
     * @brief Binds this texture to the current OpenGL context.
     */
    void bind(unsigned int slot = 0) const;

    /**
     * @brief Unbinds the texture.
     */
    void unbind() const;                    

    // TODO these should be in the OpenGLTexture class
    GLuint getID() const { return m_textureID; } // Returns the OpenGL texture ID

    // TODO dont think we need this at all
    const std::string& getPath() const { return m_filePath; }

private:
    // TODO move this into an abstraction
    GLuint m_textureID;
    GLenum m_textureType;    
    // TODO do we need to store the path??
    std::string m_filePath;  
    // this is the raw data of the image
    unsigned char* m_data;
    // store the width, height and number of channels of the image
    unsigned int m_width, m_height, m_channels;

    void loadFromFile(const std::string& filePath); // Loads texture data from file
    void loadFromMemory(unsigned char* data, size_t size);

};

#endif //TEXTURE_H
