//
// Created by Shaun on 5/10/2024.
// modified by Shaun on 30/03/2025
//

#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <unordered_map>
#include <glm/fwd.hpp>

class Shader
{
private:
    std::string m_VSFilePath;
    std::string m_FSFilePath;
    unsigned int m_shaderID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
public:
    Shader::Shader(const std::string& vs_filepath, const std::string& fs_filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniform1f(const std::string& name, float value);
    void SetUniform1i(const std::string& name, int value);
    void SetUniform2f(const std::string& name, const glm::vec2& value);
    void SetUniform3f(const std::string& name, const glm::vec3& value);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

    unsigned int GetShaderID() const { return m_shaderID; }

    //TODO fix this - very dirty
    std::string shaderDir = "source/Engine/Shaders/";

private:
    int GetUniformLocation(const std::string& name);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    std::string ParseShader(const std::string& filepath);
};



#endif //SHADER_H
