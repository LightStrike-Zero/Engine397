/**
 * @file TextureManager.cpp
 * @brief Class implementation for the TextureManager.
 *
 * @author Shaun Matthews
 * @date Created: 4/09/2024
 * @date Modified: 1/12/2024
 */

#include "TextureManager.h"

#include <assimp/texture.h>

#include "TextureLoader.h"

TextureManager& TextureManager::getInstance() {
	static TextureManager instance;
	return instance;
}

GLuint TextureManager::getTexture(const std::string& filePath) {
	auto it = m_textureCache.find(filePath);
	if (it != m_textureCache.end()) {
		return it->second->getID(); // Return the OpenGL texture ID
	}
	return 0; // Texture not found
}

GLuint TextureManager::loadTexture(const std::string& filePath) {
	// Check if the texture is already cached
	auto it = m_textureCache.find(filePath);
	if (it != m_textureCache.end()) {
		return it->second->getID();
	}

	// Load the texture
	Texture* texture = TextureLoader::loadTexture(filePath);
	if (texture) {
		m_textureCache[filePath] = texture; // Cache the texture
		return texture->getID();
	}
	return 0; // Failed to load texture
}

void TextureManager::clear() {
	for (auto& pair : m_textureCache) {
		delete pair.second; // Free texture memory
	}
	m_textureCache.clear();
}

TextureManager::~TextureManager() {
	clear(); // Ensure all textures are freed
}

GLuint TextureManager::loadEmbeddedTexture(aiTexture* embeddedTexture) {
	// Generate a unique key for the embedded texture
	std::string textureKey = "embedded_" + std::to_string(reinterpret_cast<uintptr_t>(embeddedTexture));

	// Check if the texture is already cached
	auto it = m_textureCache.find(textureKey);
	if (it != m_textureCache.end()) {
		return it->second->getID();
	}

	// Load the texture data
	Texture* texture = TextureLoader::loadEmbeddedTexture(embeddedTexture);
	if (texture) {
		m_textureCache[textureKey] = texture; // Cache the texture
		return texture->getID();
	}
	return 0; // Failed to load texture
}
