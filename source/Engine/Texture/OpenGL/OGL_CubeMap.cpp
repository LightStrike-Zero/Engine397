#include "OGL_CubeMap.h"
#include <iostream>
#include "stb_image.h"

OpenGLCubeMap::OpenGLCubeMap(const std::array<std::string, 6>& faces)
    : m_textureID(0), m_width(0), m_height(0)
{
    // Generate the OpenGL texture for the cubemap.
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

    // Load each face of the cubemap.
    for (unsigned int i = 0; i < faces.size(); i++) {
        int width, height, channels;
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
        if (data) {
            // Save dimensions from the first loaded face
            if (i == 0) {
                m_width = static_cast<uint32_t>(width);
                m_height = static_cast<uint32_t>(height);
            }

            GLenum format;
            if (channels == 1)
                format = GL_RED;
            else if (channels == 3)
                format = GL_RGB;
            else if (channels == 4)
                format = GL_RGBA;
            else
                format = GL_RGB; // Fallback

            // Load the texture data for the face.
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cerr << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    // Set parameters for the cubemap.
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Unbind the cubemap.
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

OpenGLCubeMap::~OpenGLCubeMap() {
    glDeleteTextures(1, &m_textureID);
}

void OpenGLCubeMap::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
}

void OpenGLCubeMap::unbind() const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
