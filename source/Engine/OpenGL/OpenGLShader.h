//
// Created by Shaun on 15/03/2025.
//

/**
 * @file OpenGLShader.h
 * @brief Defines the OpenGLShader class, which handles OpenGL shader program management.
 * @author Shaun
 * @date 2025-03-15
 */

#ifndef OPENGLSHADER_H
#define OPENGLSHADER_H
#include <unordered_map>

#include "Interfaces/IShader.h"

/// @struct ShaderProgramSource
/// @brief Holds source code strings for vertex and fragment shaders.
struct ShaderProgramSource
{
    std::string VertexSource;             ///< Vertex shader source code
    std::string FragmentSource;           ///< Fragment shader source code
};

/**
 * @class OpenGLShader
 * @brief Implements the IShader interface using OpenGL for shader operations.
 */
class OpenGLShader : public IShader {
public:
    /**
     * @brief Constructor that loads and compiles shaders from the given file paths.
     * @param vertexPath Path to the vertex shader file.
     * @param fragmentPath Path to the fragment shader file.
     */
    OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath);

    /// @brief Destructor.
    ~OpenGLShader() override;

    /// @brief Binds the shader program for use.
    void bind() override;

    /// @brief Unbinds the current shader program.
    void unbind() override;

    /// @brief Sets an integer uniform variable.
    void SetUniform1i(const std::string& name, int value) override;

    /// @brief Sets a float uniform variable.
    void SetUniform1f(const std::string& name, float value) override;

    /// @brief Sets a vec2 uniform variable.
    void SetUniform2f(const std::string& name, const glm::vec2& value) override;

    /// @brief Sets a vec3 uniform variable.
    void SetUniform3f(const std::string& name, const glm::vec3& value) override;

    /// @brief Sets a vec4 uniform variable.
    void SetUniform4f(const std::string& name, const glm::vec4& value) override;

    /// @brief Sets a mat3 uniform variable.
    void SetUniformMat3(const std::string& name, const glm::mat3& matrix) override;

    /// @brief Sets a mat4 uniform variable.
    void SetUniformMat4(const std::string& name, const glm::mat4& matrix) override;

    /// @brief Gets the OpenGL ID of the shader program.
    /// @return The OpenGL shader program ID.
    [[nodiscard]] unsigned int getID() const override { return m_shaderID; }

private:
    std::string m_VSFilePath;
    std::string m_FSFilePath;
    unsigned int m_shaderID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
    
    //TODO fix this - very dirty
    std::string shaderDir = "source/Engine/Shaders/";

    int GetUniformLocation(const std::string& name);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    std::string ParseShader(const std::string& filepath);
};

#endif //OPENGLSHADER_H
