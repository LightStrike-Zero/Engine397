/**
* @file TextureManager.h
 * @brief Class definition for the TextureManager.
 *
 * The TextureManager class is responsible for managing textures in the application.
 * It provides functionality to load textures from files, raw image data, or memory,
 * retrieve cached textures, and clear the texture cache.
 *
 * @details
 * - Uses a singleton pattern to ensure a single instance of the manager.
 * - Caches textures to avoid redundant loading.
 * - Supports loading textures from various sources.
 * 
 * @author Shaun Matthews
 * @date Created: 4/09/2024
 * @date Modified: 1/12/2024
 * @date Modified: 30/03/2025
 * @date Modified: 10/04/2025
 * @date Modified: 12/04/2025
 * Removed reference to OpenGL types, now using uint32_t for texture IDs,
 * since uint32_t is a standard type
 * 
 */

#pragma once

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H


#include <string>
#include <memory>
#include <unordered_map>

#include "ImageData.h"

class ITexture;


/**
 * @class TextureManager
 * @brief Singleton class for managing textures.
 *
 * The TextureManager handles loading, caching, and retrieving textures.
 * It ensures efficient texture management by avoiding redundant loading
 * and providing a centralized interface for texture operations.
 */
class TextureManager {
public:
	
	/**
	 * @brief Get the singleton instance of the TextureManager.
	 * @return Reference to the TextureManager instance.
	 */
	static TextureManager& getInstance();

	/**
	 * @brief Load a texture from a file on disk.
	 * @param filePath The file path to the texture file.
	 * @return The ID of the loaded texture as uint32_t.
	 */
	uint32_t  loadTextureFromFile(const std::string& filePath);

	/**
	 * @brief Load a texture from raw image data, used typically when passing an embedded image from a model.
	 * @param imageData The raw image data to load the texture from.
	 * @return The ID of the loaded texture as uint32_t.
	 */
	uint32_t  loadTextureFromData(const RawImageData& imageData);

	/**
	 * @brief Load a texture from stored in memory.
	 * @param data Pointer to the raw memory containing the texture data.
	 * @param size The size of the memory block.
	 * @return The ID of the loaded texture as uint32_t.
	 */
	uint32_t  loadTextureFromMemory(unsigned char* data, size_t size);

	/**
	 * @brief Retrieve a texture from the cache.
	 * @param key The key used to identify the texture (e.g., file path).
	 * @return A shared pointer to the cached texture, or nullptr if not found so check for nullptr.
	 */
	std::shared_ptr<ITexture> getTexture(const std::string& key);

	/**
	* @brief Clear the texture cache.
	*
	* Removes all cached textures from memory.
	*/
	void clear();

	// Delete copy constructor and assignment operator to enforce singleton pattern
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;
private:
	/**
	 * @brief Private constructor for the TextureManager.
	 *
	 * Ensures that the class can only be instantiated through getInstance().
	 */
	TextureManager() = default;

	/**
	 * @brief Destructor for the TextureManager.
	 */
	~TextureManager();

	std::unordered_map<std::string, std::shared_ptr<ITexture>> m_textureCache;	///< Cache for storing textures.

};

#endif // !TEXTURE_MANAGER_H
