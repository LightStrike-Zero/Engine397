/**
 * @file TextureManager.h
 * @brief Class definition for the TextureManager.
 *
 * @author Shaun Matthews
 * @date Created: 4/09/2024
 * @date Modified: 1/12/2024
 */

#pragma once

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H


#include <GL/glew.h>
#include <string>
#include <texture.h>
#include <unordered_map>
#include <assimp/texture.h>

class TextureManager
{
public:
	static TextureManager& getInstance();

	GLuint getTexture(const std::string& filePath);
	GLuint loadTexture(const std::string& filePath);
	GLuint loadEmbeddedTexture(aiTexture* embeddedTexture);
	void clear(); // Clears the cache

private:
	TextureManager() = default;
	~TextureManager();

	std::unordered_map<std::string, Texture*> m_textureCache; // Map of texture ID -> Texture*

	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;
};

#endif // !TEXTURE_MANAGER_H
