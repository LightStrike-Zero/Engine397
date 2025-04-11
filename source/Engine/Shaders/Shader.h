//
// Created by Shaun on 5/10/2024.
// modified by Shaun on 30/03/2025
//

/**
 * @file Shader.h
 * @brief Defines the Shader class for compiling and managing GLSL shaders.
 *
 * This class provides functionality to load, compile, bind, and manage
 * uniform variables in vertex and fragment shaders.
 *
 * @author Shaun
 * @date Created: 2024-10-05
 * @date Modified: 2025-03-30
 */

#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <unordered_map>
#include <glm/fwd.hpp>

/**
 * @class Shader
 * @brief Encapsulates an OpenGL shader program and its uniforms.
 */
class Shader
{
private:
    std::string m_VSFilePath;                                            ///< Path to vertex shader
    std::string m_FSFilePath;                                            ///< Path to fragment shader
    unsigned int m_shaderID;                                             ///< OpenGL shader program ID
    std::unordered_map<std::string, int> m_UniformLocationCache;         ///< Cache for uniform locations

public:
    /**
     * @brief Constructs a Shader object from given vertex and fragment shader file paths.
     */
    Shader::Shader(const std::string& vs_filepath, const std::string& fs_filepath);

    /**
     * @brief Destructor that deletes the shader program.
     */
    ~Shader();

    /// @brief Binds the shader program for use.
    void Bind() const;

    /// @brief Unbinds the shader program.
    void Unbind() const;

    /// @brief Sets a float uniform.
    void SetUniform1f(const std::string& name, float value);

    /// @brief Sets an integer uniform.
    void SetUniform1i(const std::string& name, int value);

    /// @brief Sets a 2D float vector uniform.
    void SetUniform2f(const std::string& name, const glm::vec2& value);

    /// @brief Sets a 3D float vector uniform.
    void SetUniform3f(const std::string& name, const glm::vec3& value);

    /// @brief Sets a 4-component float uniform.
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

    /// @brief Sets a 4x4 matrix uniform.
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

    /// @brief Returns the internal OpenGL shader ID.
    unsigned int GetShaderID() const { return m_shaderID; }

    //TODO fix this - very dirty
    /// @brief (Temporary) Path to shaders, currently hardcoded.
    std::string shaderDir = "source/Engine/Shaders/";

private:
    int GetUniformLocation(const std::string& name);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    std::string ParseShader(const std::string& filepath);
};

#endif //SHADER_H
