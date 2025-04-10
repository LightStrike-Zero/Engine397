//
// Created by Shaun on 11/04/2025.
//

#ifndef OGL_TEXTURE_H
#define OGL_TEXTURE_H

#include "Interfaces/ITexture.h"
#include <string>
#include <GL/glew.h>

class OpenGLTexture final : public ITexture {
public:
    OpenGLTexture();
    explicit OpenGLTexture(const std::string& filePath, GLenum textureType = GL_TEXTURE_2D);
    explicit OpenGLTexture(unsigned char* data, size_t size, GLenum textureType = GL_TEXTURE_2D);
    ~OpenGLTexture() override;

    void bind(unsigned int slot) const override;
    void unbind() const override;
    [[nodiscard]] uint32_t getWidth() const override { return m_width; }
    [[nodiscard]] uint32_t getHeight() const override { return m_height; }
    [[nodiscard]] uint8_t getChannels() const override { return m_channels; }
    
    [[nodiscard]] GLuint getID() const { return m_textureID; }

    [[nodiscard]] const std::string& getPath() const { return m_filePath; }

protected:
    void loadFromFile(const std::string& filePath);
    void loadFromMemory(unsigned char* data, size_t size);
    void loadImageData(const std::string& filePath);

private:
    GLuint m_textureID;
    GLenum m_textureType;
    std::string m_filePath;
    unsigned char* m_data;
    uint8_t m_channels;
    uint32_t m_width, m_height;
};

#endif //OGL_TEXTURE_H
