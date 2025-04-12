// TextureFactory.cpp
#include "TextureFactory.h"
#include "Texture/OpenGL/OGL_Texture.h"
#include <iostream>

std::shared_ptr<ITexture> TextureFactory::createTexture() {
    return std::make_shared<OpenGLTexture>();
}

std::shared_ptr<ITexture> TextureFactory::createTexture(const std::string& filePath) {
    return std::make_shared<OpenGLTexture>(filePath);
}

std::shared_ptr<ITexture> TextureFactory::createTexture(unsigned char* data, size_t size) {
    return std::make_shared<OpenGLTexture>(data, size);
}

std::shared_ptr<ITexture> TextureFactory::createTexture(const RawImageData& imageData) {
    return std::make_shared<OpenGLTexture>(imageData);
}