//
// Created by Shaun on 11/04/2025.
//

#include "OGL_Texture.h"


#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

OpenGLTexture::OpenGLTexture(): m_textureID(0), m_textureType(0), m_data(nullptr), m_width(0), m_height(0),
                                m_channels(0)
{
}

OpenGLTexture::OpenGLTexture(const std::string& filePath, GLenum textureType)
    : m_filePath(filePath), m_textureType(textureType), m_textureID(0)
{
    loadFromFile(filePath);
}

OpenGLTexture::OpenGLTexture(unsigned char* data, size_t size, GLenum textureType)
    : m_textureType(textureType), m_textureID(0)
{
    loadFromMemory(data, size);
}

OpenGLTexture::OpenGLTexture(const RawImageData& imageData, GLenum textureType)
    : m_textureID(0), m_textureType(textureType), m_filePath(""), m_data(nullptr), m_channels(0), m_width(0),
      m_height(0)
{
    loadFromRawData(imageData);
}


OpenGLTexture::~OpenGLTexture()
{
    glDeleteTextures(1, &m_textureID);
}

void OpenGLTexture::bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(m_textureType, m_textureID);
}

void OpenGLTexture::unbind() const
{
    glBindTexture(m_textureType, 0);
}

void OpenGLTexture::loadFromFile(const std::string& filePath)
{
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

    int width, height, channels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

    if (data)
    {
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB; // Determine format
        glTexImage2D(m_textureType, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(m_textureType);
        std::cout << "Loaded texture: " << filePath << " (" << width << "x" << height << ", " << channels << " channels)" << std::endl;
    }
    else
    {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
    }

    stbi_image_free(data);
    glBindTexture(m_textureType, 0);
}

void OpenGLTexture::loadFromMemory(unsigned char* data, size_t size)
{
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

    if (imageData)
    {
        GLenum format, internalFormat;
        if (channels == 1)
        {
            internalFormat = GL_R16;
            format = GL_RED;
        }
        else if (channels == 3)
        {
            internalFormat = GL_RGB;
            format = GL_RGB;
        }
        else if (channels == 4)
        {
            internalFormat = GL_RGBA;
            format = GL_RGBA;
        }
        else
        {
            internalFormat = GL_RGB;
            format = GL_RGB;
        }

        glTexImage2D(m_textureType, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(m_textureType);

        if (channels == 1)
        {
            GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, 0};
            glTexParameteriv(m_textureType, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
        }

        std::cout << "Loaded texture from memory." << channels << std::endl;
    }
    else
    {
        std::cerr << "Failed to load texture from memory." << std::endl;
    }

    stbi_image_free(imageData);
    glBindTexture(m_textureType, 0);
}

void OpenGLTexture::loadImageData(const std::string& filePath)
{
    int width, height, channels;
    unsigned char* imageData = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
    if (imageData)
    {
        m_data = imageData;
        m_width = width;
        m_height = height;
        m_channels = channels;
    }
    else
    {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
    }
}

void OpenGLTexture::loadFromRawData(const RawImageData& imageData)
{
    if (imageData.isCompressed)
    {
        loadFromMemory(imageData.data, imageData.dataSize);
    }
    else
    {
        glGenTextures(1, &m_textureID);
        glBindTexture(m_textureType, m_textureID);

        glTexParameteri(m_textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(m_textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(m_textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int channels = imageData.dataSize / (imageData.width * imageData.height);
        m_width = imageData.width;
        m_height = imageData.height;
        m_channels = channels;

        // Determine format
        GLenum format, internalFormat;
        if (channels == 1)
        {
            internalFormat = GL_R16;
            format = GL_RED;
        }
        else if (channels == 3)
        {
            internalFormat = GL_RGB;
            format = GL_RGB;
        }
        else if (channels == 4)
        {
            internalFormat = GL_RGBA;
            format = GL_RGBA;
        }
        else
        {
            internalFormat = GL_RGB;
            format = GL_RGB;
        }

        glTexImage2D(m_textureType, 0, internalFormat, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, imageData.data);
        glGenerateMipmap(m_textureType);

        if (channels == 1)
        {
            GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, 0};
            glTexParameteriv(m_textureType, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
        }

        glBindTexture(m_textureType, 0);

        std::cout << "Loaded texture from raw data: " << m_width << "x" << m_height << ", " << channels << " channels" << std::endl;
    }
}
