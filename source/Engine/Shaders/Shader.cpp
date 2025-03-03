
#include "Shader.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "Renderer.h"

std::string shaderDir = "Z:\\Code Projects\\CarbonEngine\\source\\Engine\\Shaders\\";

Shader::Shader(const std::string& vs_filepath, const std::string& fs_filepath)
    : m_VSFilePath(vs_filepath), m_FSFilePath(fs_filepath)
{

    ShaderProgramSource source;
    source.FragmentSource = ParseShader(shaderDir + fs_filepath);
    std::cout << "Fragment Shader path: " << shaderDir + fs_filepath << std::endl;
    source.VertexSource = ParseShader(shaderDir +  vs_filepath);
    std::cout << "Vertex Shader path: " << shaderDir + vs_filepath << std::endl;
    m_shaderID = CreateShader(source.VertexSource, source.FragmentSource);

}

Shader::~Shader()
{
    glDeleteProgram(m_shaderID);
}

void Shader::Bind() const
{
    glUseProgram(m_shaderID);
    std::cout << "Shader ID we bound: " << m_shaderID << std::endl;
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

/**
 * Sets a vec4 uniform.
 */
void Shader::SetUniform4f(const std::string& name, const float v0, const float v1, const float v2, const float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

/**
 * Sets a mat4 uniform.
 */
void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

/**
 * Sets an integer uniform.
 */
void Shader::SetUniform1i(const std::string& name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}
/**
 * Sets a vec2 uniform.
 */
void Shader::SetUniform2f(const std::string& name, const glm::vec2& value)
{
    glUniform2f(GetUniformLocation(name), value.x, value.y);
}
/**
 * Sets a vec3 uniform.
 */
void Shader::SetUniform3f(const std::string& name, const glm::vec3& value)
{
    glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

/**
 * Sets a float uniform.
 */
void Shader::SetUniform1f(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

int Shader::GetUniformLocation(const std::string& name)
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


unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
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

std::string Shader::ParseShader(const std::string& filepath)
{
    std::ifstream file(filepath);
    std::string str;
    std::string shader;
    while (std::getline(file, str)) {
        shader.append(str + "\n");
    }
    return shader;
}


