//
// Created by Shaun on 15/03/2025.
//

#ifndef ISHADER_H
#define ISHADER_H
#include <string>
#include <glm/fwd.hpp>

class IShader
{
public:
    virtual ~IShader() = default;
    virtual void bind() = 0;
    virtual void unbind() = 0;

    // Uniform setters
    virtual void SetUniform1i(const std::string& name, int value) = 0;
    virtual void SetUniform1f(const std::string& name, float value) = 0;
    virtual void SetUniform2f(const std::string& name, const glm::vec2& value) = 0;
    virtual void SetUniform3f(const std::string& name, const glm::vec3& value) = 0;
    virtual void SetUniform4f(const std::string& name, const glm::vec4& value) = 0;
    virtual void SetUniformMat3(const std::string& name, const glm::mat3& matrix) = 0;
    virtual void SetUniformMat4(const std::string& name, const glm::mat4& matrix) = 0;
    
    [[nodiscard]] virtual unsigned int getID() const = 0;
};

#endif //ISHADER_H
