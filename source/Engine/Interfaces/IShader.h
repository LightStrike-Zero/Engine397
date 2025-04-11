//
// Created by Shaun on 15/03/2025.
//

/**
 * @file IShader.h
 * @brief Interface for handling shader programs.
 * @author Shaun
 * @date 2025-03-15
 */

#ifndef ISHADER_H
#define ISHADER_H
#include <string>
#include <glm/fwd.hpp>

/**
 * @class IShader
 * @brief Abstract interface for shader management.
 * Provides methods to bind/unbind shaders and set uniform values.
 */
class IShader
{
public:
    virtual ~IShader() = default;

    /// Binds the shader program for rendering.
    virtual void bind() = 0;

    /// Unbinds the current shader program.
    virtual void unbind() = 0;

    // Uniform setters

    /// Sets an integer uniform.
    virtual void SetUniform1i(const std::string& name, int value) = 0;

    /// Sets a float uniform.
    virtual void SetUniform1f(const std::string& name, float value) = 0;

    /// Sets a 2D float vector uniform.
    virtual void SetUniform2f(const std::string& name, const glm::vec2& value) = 0;

    /// Sets a 3D float vector uniform.
    virtual void SetUniform3f(const std::string& name, const glm::vec3& value) = 0;

    /// Sets a 4D float vector uniform.
    virtual void SetUniform4f(const std::string& name, const glm::vec4& value) = 0;

    /// Sets a 3x3 matrix uniform.
    virtual void SetUniformMat3(const std::string& name, const glm::mat3& matrix) = 0;

    /// Sets a 4x4 matrix uniform.
    virtual void SetUniformMat4(const std::string& name, const glm::mat4& matrix) = 0;

    /**
     * @brief Gets the internal shader program ID.
     * @return The OpenGL ID of the shader program.
     */
    [[nodiscard]] virtual unsigned int getID() const = 0;
};

#endif //ISHADER_H
