//
// Created by Shaun on 10/02/2025.
//

/**
 * @file ShaderManager.h
 * @brief Manages and stores shaders used throughout the application.
 *
 * Provides functionality to load and retrieve reusable shader programs,
 * wrapping OpenGLShader objects.
 *
 * @author Shaun
 * @date Created: 2025-02-10
 */

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include <memory>
#include <unordered_map>

#include "OpenGL/OpenGLShader.h"

/**
 * @class ShaderManager
 * @brief Central manager for loading and accessing OpenGL shaders.
 */
class ShaderManager {

private:
    std::unordered_map<std::string, std::shared_ptr<OpenGLShader>> m_shaders;

public:
    /**
     * @brief Retrieves a shader by name.
     * @param shaderName The name used to store the shader.
     * @return A shared pointer to the OpenGLShader.
     */
    std::shared_ptr<OpenGLShader> getShader(const std::string& shaderName) {
        return m_shaders.at(shaderName);
    }

    /**
     * @brief Loads and stores a shader from given file paths.
     * @param shaderName The name to associate with the shader.
     * @param vertexPath Path to the vertex shader file.
     * @param fragmentPath Path to the fragment shader file.
     */
    void loadShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
    {
        m_shaders.emplace(shaderName, std::make_shared<OpenGLShader>(vertexPath, fragmentPath));
    }
};

#endif //SHADERMANAGER_H
