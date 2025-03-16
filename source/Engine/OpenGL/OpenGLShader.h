//
// Created by Shaun on 15/03/2025.
//

#ifndef OPENGLSHADER_H
#define OPENGLSHADER_H
#include <unordered_map>

#include "Interfaces/IShader.h"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;

};

class OpenGLShader : public IShader {
public:
    OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath);
    ~OpenGLShader() override;

    void bind() override;
    void unbind() override;
    
    void SetUniform1i(const std::string& name, int value) override;
    void SetUniform1f(const std::string& name, float value) override;
    void SetUniform2f(const std::string& name, const glm::vec2& value) override;
    void SetUniform3f(const std::string& name, const glm::vec3& value) override;
    void SetUniform4f(const std::string& name, const glm::vec4& value) override;
    void SetUniformMat3(const std::string& name, const glm::mat3& matrix) override;
    void SetUniformMat4(const std::string& name, const glm::mat4& matrix) override;
    
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
