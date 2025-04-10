/**
 * @file TextureManager.h
 * @brief Class definition for the TextureManager.
 *
 * @author Shaun Matthews
 * @date Created: 4/09/2024
 * @date Modified: 1/12/2024
 * @date Modified: 30/03/2025
 * @date Modified: 10/04/2025
 * Removed reference to OpenGL types, now references abstract texture class
 * 
 */

#pragma once

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H


#include <string>
#include <memory>
#include <unordered_map>

class ITexture;

class TextureManager
{
public:
	static TextureManager& getInstance();

	std::shared_ptr<ITexture> getTexture(const std::string& filePath);
	std::shared_ptr<ITexture> loadTexture(const std::string& filePath);
	std::shared_ptr<ITexture> loadEmbeddedTexture(void* embeddedTexture);
	void clear(); 

private:
	TextureManager() = default;
	~TextureManager();

	std::unordered_map<std::string, std::shared_ptr<ITexture>> m_textureCache; // Map of texture ID -> Texture*

	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;
};

#endif // !TEXTURE_MANAGER_H
