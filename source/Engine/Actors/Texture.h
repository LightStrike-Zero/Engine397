//
// Created by Shaun on 06/10/2024.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>

// struct Texture
// {
//     unsigned int id;
//     std::string type;
// };

class Texture {
public:
    explicit Texture(const std::string& filePath, GLenum textureType = GL_TEXTURE_2D);
    explicit Texture(unsigned char* data, size_t size, GLenum textureType = GL_TEXTURE_2D);
    ~Texture();

    void bind(unsigned int slot = 0) const; // Binds texture to a texture unit
    void unbind() const;                    // Unbinds texture

    GLuint getID() const { return m_textureID; } // Returns the OpenGL texture ID
    const std::string& getPath() const { return m_filePath; }

private:
    GLuint m_textureID;      // OpenGL texture ID
    std::string m_filePath;  // Path to the texture file
    GLenum m_textureType;    // Texture type (e.g., GL_TEXTURE_2D)

    void loadFromFile(const std::string& filePath); // Loads texture data from file
    void loadFromMemory(unsigned char* data, size_t size);
};


#endif //TEXTURE_H
