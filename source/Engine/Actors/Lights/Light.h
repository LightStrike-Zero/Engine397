//
// Created by Shaun on 8/12/2024.
//

#ifndef LIGHT_H
#define LIGHT_H

#include <glm/vec3.hpp>

class Light
{
public:
    virtual ~Light() = default;

    virtual void setAmbient(const glm::vec3& ambient);
    virtual void setDiffuse(const glm::vec3& diffuse);
    virtual void setSpecular(const glm::vec3& specular);
    [[nodiscard]] virtual glm::vec3 getAmbient() const;
    [[nodiscard]] virtual glm::vec3 getDiffuse() const;
    [[nodiscard]] virtual glm::vec3 getSpecular() const;
protected:
    glm::vec3 ambient{};
    glm::vec3 diffuse{};
    glm::vec3 specular{};

};

#endif //LIGHT_H
