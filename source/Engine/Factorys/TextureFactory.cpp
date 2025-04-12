// TextureFactory.cpp
#include "TextureFactory.h"

#include "Interfaces/ICubeMap.h"
#include "Texture/OpenGL/OGL_CubeMap.h"
#include "Texture/OpenGL/OGL_Texture.h"
#include <iostream>

std::shared_ptr<ITexture> TextureFactory::createTexture() {
    try {
    return std::make_shared<OpenGLTexture>();
    } catch (const std::exception& e) {
        std::cerr << "Texture creation failed: " << e.what() << std::endl;
        return nullptr;
    }
}

std::shared_ptr<ITexture> TextureFactory::createTexture(const std::string& filePath) {
    try {
    return std::make_shared<OpenGLTexture>(filePath);
    } catch (const std::exception& e) {
        std::cerr << "Texture creation failed: " << e.what() << std::endl;
        return nullptr;
    }
}

std::shared_ptr<ITexture> TextureFactory::createTexture(unsigned char* data, size_t size) {
    try {
    return std::make_shared<OpenGLTexture>(data, size);
    } catch (const std::exception& e) {
        std::cerr << "Texture creation failed: " << e.what() << std::endl;
        return nullptr;
    }
}

std::shared_ptr<ITexture> TextureFactory::createTexture(const RawImageData &imageData) {
    try {
    return std::make_shared<OpenGLTexture>(imageData);
    } catch (const std::exception& e) {
        std::cerr << "Texture creation failed: " << e.what() << std::endl;
        return nullptr;
    }
}

std::shared_ptr<ICubeMap> TextureFactory::createTexture(const std::array<std::string, 6>& faces) {
    try {
        return std::make_shared<OpenGLCubeMap>(faces);
    } catch (const std::exception& e) {
        std::cerr << "Cubemap creation failed: " << e.what() << std::endl;
        return nullptr;
    }
}