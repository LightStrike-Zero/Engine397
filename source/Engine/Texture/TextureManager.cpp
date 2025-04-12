// TextureManager.cpp
#include "TextureManager.h"
#include <sstream>  

#include "Factorys/TextureFactory.h"

TextureManager& TextureManager::getInstance() {
    static TextureManager instance;
    return instance;
}

uint32_t TextureManager::loadTextureFromFile(const std::string& filePath) {
    // Check cache using filePath as key
    auto it = m_textureCache.find(filePath);
    if (it != m_textureCache.end())
        return it->second->getID();

    std::shared_ptr<ITexture> texture = TextureFactory::createTexture(filePath);
    if (texture)
        m_textureCache[filePath] = texture;
    return texture ? texture->getID() : 0;
}

uint32_t TextureManager::loadTextureFromData(const RawImageData& imageData) {
    std::ostringstream keyStream;
    keyStream << "raw_" << imageData.dataSize;
    std::string key = keyStream.str();
    
    auto it = m_textureCache.find(key);
    if (it != m_textureCache.end())
        return it->second->getID();
    
    std::shared_ptr<ITexture> texture = TextureFactory::createTexture(imageData);
    if (texture)
        m_textureCache[key] = texture;
    return texture ? texture->getID() : 0;
}

uint32_t TextureManager::loadTextureFromMemory(unsigned char* data, size_t size) {
    std::ostringstream keyStream;
    keyStream << "mem_" << size << "_";
    std::string key = keyStream.str();
    
    auto it = m_textureCache.find(key);
    if (it != m_textureCache.end())
        return it->second->getID();
    
    std::shared_ptr<ITexture> texture = TextureFactory::createTexture(data, size);
    if (texture)
        m_textureCache[key] = texture;
    return texture ? texture->getID() : 0;
}

void TextureManager::clear() {
    m_textureCache.clear();
}

TextureManager::~TextureManager() {
    clear();
}