//
// Created by Shaun on 10/02/2025.
//

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include <memory>
#include <unordered_map>

#include "OpenGL/OpenGLShader.h"


class ShaderManager {
private:
    std::unordered_map<std::string, std::shared_ptr<OpenGLShader>> m_shaders;
public:
    std::shared_ptr<OpenGLShader> getShader(const std::string& shaderName) {
        return m_shaders.at(shaderName);
    }

    void loadShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
    {
        m_shaders.emplace(shaderName, std::make_shared<OpenGLShader>(vertexPath, fragmentPath));
    }
};



#endif //SHADERMANAGER_H
