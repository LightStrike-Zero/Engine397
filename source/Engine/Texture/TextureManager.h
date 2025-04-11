/**
 * @file TextureManager.h
 * @brief Class definition for the TextureManager singleton.
 *
 * Handles loading and caching of texture assets using OpenGL.
 * Prevents duplicate loading of the same file and manages memory cleanup.
 *
 * @author Shaun Matthews
 * @date Created: 4/09/2024
 * @date Modified: 1/12/2024
 * @date Modified: 30/03/2025
 */

#pragma once

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H


#include <GL/glew.h>
#include <string>
#include <texture/texture.h>
#include <unordered_map>
#include <assimp/texture.h>

/**
 * @class TextureManager
 * @brief Singleton class that manages loading and caching of textures.
 */
class TextureManager
{
public:

    /**
     * @brief Retrieves the singleton instance of the TextureManager.
     * @return Reference to the TextureManager instance.
     */
	static TextureManager& getInstance();

    /**
     * @brief Retrieves the OpenGL texture ID for the given file path.
     * If not already loaded, the texture will be loaded and cached.
     * @param filePath Path to the texture image.
     * @return OpenGL texture ID.
     */
	GLuint getTexture(const std::string& filePath);

    /**
     * @brief Loads a texture and returns its OpenGL ID.
     * @param filePath Path to the texture image.
     * @return OpenGL texture ID.
     */
	GLuint loadTexture(const std::string& filePath);

    /**
     * @brief Loads an embedded texture from an Assimp aiTexture.
     * @param embeddedTexture Pointer to the embedded texture.
     * @return OpenGL texture ID.
     */
	GLuint loadEmbeddedTexture(aiTexture* embeddedTexture);

    /**
     * @brief Clears all loaded textures and frees memory.
     */
	void clear(); // Clears the cache

private:
	TextureManager() = default;
	~TextureManager();

	std::unordered_map<std::string, Texture*> m_textureCache; // Map of texture ID -> Texture*

	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;
};

#endif // !TEXTURE_MANAGER_H
