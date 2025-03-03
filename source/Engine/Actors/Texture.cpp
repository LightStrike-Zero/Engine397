//
// Created by Shaun on 17/07/2024.
//

#include <iostream>

#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& filePath, GLenum textureType)
    : m_filePath(filePath), m_textureType(textureType), m_textureID(0) {
    loadFromFile(filePath);
}

Texture::Texture(unsigned char* data, size_t size, GLenum textureType)
    : m_textureType(textureType), m_textureID(0) {
    loadFromMemory(data, size);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_textureID);
}

void Texture::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(m_textureType, m_textureID);
    // std::cout << "Binding texture ID: " << m_textureID << " to slot: " << slot << std::endl;
}

void Texture::unbind() const {
    glBindTexture(m_textureType, 0);
}

void Texture::loadFromFile(const std::string& filePath) {
    // Generate texture
    glGenTextures(1, &m_textureID);
    glBindTexture(m_textureType, m_textureID);

    // Texture parameters
    glTexParameteri(m_textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(m_textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(m_textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))
    {
        GLfloat maxAnisotropic = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropic);
        glTexParameterf(m_textureType, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropic);
    }

    // Load image using stb_image
    int width, height, channels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

    if (data) {
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB; // Determine format
        glTexImage2D(m_textureType, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(m_textureType);
    }
    else {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
    }

    stbi_image_free(data);
    glBindTexture(m_textureType, 0);
}

void Texture::loadFromMemory(unsigned char* data, size_t size) {
    // Generate texture
    glGenTextures(1, &m_textureID);
    glBindTexture(m_textureType, m_textureID);

    // Texture parameters
    glTexParameteri(m_textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(m_textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(m_textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image using stb_image from memory
    int width, height, channels;
    unsigned char* imageData = stbi_load_from_memory(data, static_cast<int>(size), &width, &height, &channels, 0);

    if (imageData) {
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB; // Determine format
        glTexImage2D(m_textureType, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(m_textureType);
        std::cout << "Loaded texture from memory." << std::endl;
    } else {
        std::cerr << "Failed to load texture from memory." << std::endl;
    }

    stbi_image_free(imageData);
    glBindTexture(m_textureType, 0);
}