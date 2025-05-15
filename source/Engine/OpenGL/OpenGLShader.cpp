//
// Created by Shaun on 15/03/2025.
//

#include "OpenGLShader.h"
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

OpenGLShader::OpenGLShader(const std::string& vs_filepath, const std::string& fs_filepath)
    : m_VSFilePath(vs_filepath), m_FSFilePath(fs_filepath)
{
//    std::cout << "[vs_filepath]" << vs_filepath << std::endl; // buko - this works
//    std::cout << "[fs_filepath]" << fs_filepath << std::endl; // buko - this works

    ShaderProgramSource source;
    source.FragmentSource = ParseShader(shaderDir + fs_filepath);
    source.VertexSource = ParseShader(shaderDir +  vs_filepath);
    m_shaderID = CreateShader(source.VertexSource, source.FragmentSource);
}

OpenGLShader::~OpenGLShader()
{
    glDeleteProgram(m_shaderID);
}

void OpenGLShader::bind() 
{
    glUseProgram(m_shaderID);
}

void OpenGLShader::unbind() 
{
    glUseProgram(0);
}

/**
 * Sets a vec4 uniform.
 */
void OpenGLShader::SetUniform4f(const std::string& name, const glm::vec4& value)
{
    glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void OpenGLShader::SetUniformMat3(const std::string& name, const glm::mat3& matrix)
{
    glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

/**
 * Sets a mat4 uniform.
 */
void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

/**
 * Sets an integer uniform.
 */
void OpenGLShader::SetUniform1i(const std::string& name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}
/**
 * Sets a vec2 uniform.
 */
void OpenGLShader::SetUniform2f(const std::string& name, const glm::vec2& value)
{
    glUniform2f(GetUniformLocation(name), value.x, value.y);
}
/**
 * Sets a vec3 uniform.
 */
void OpenGLShader::SetUniform3f(const std::string& name, const glm::vec3& value)
{
    glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

/**
 * Sets a float uniform.
 */
void OpenGLShader::SetUniform1f(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

int OpenGLShader::GetUniformLocation(const std::string& name)
{

    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[name];
    }

    int location = glGetUniformLocation(m_shaderID, name.c_str());
    if(location == -1)
    {
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    }
    m_UniformLocationCache[name] = location;

    return location;
}


unsigned int OpenGLShader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    // buko testing
//    std::cout << "[ID]" << id << std::endl;
//    std::cout << "[SOURCE]" << source << std::endl;
//    std::cout << "[SRC]" << src << std::endl;

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // buko testing the paths
    //std::string thing = ParseShader("source/Engine/Shaders/HUD.frag");
    //std::cout << "[DEBUG] Shader source (" << "source/Engine/Shaders/HUD.frag" << "):\n" << thing << std::endl;

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "ERROR: Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int OpenGLShader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();

//    std::cout << "[DEBUG] Vertex Shader: " << vertexShader << std::endl;   // buko test
//    std::cout << "[DEBUG] Fragment Shader: " << fragmentShader << std::endl; // buko test

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

std::string OpenGLShader::ParseShader(const std::string& filepath)
{
    std::ifstream file(filepath);
    std::string str;
    std::string shader;
    while (std::getline(file, str)) {
        shader += str + "\n";
    }
    return shader;
}