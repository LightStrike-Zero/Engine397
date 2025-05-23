//
// Created by Shaun on 11/02/2025.
//

#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include <glm/vec3.hpp>

#include "Light.h"


class DirectionalLight : public Light
{
public:
    DirectionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse,
                     const glm::vec3& specular, const float specular_power)

    {
        DirectionalLight::setAmbient(ambient);
        DirectionalLight::setDiffuse(diffuse);
        DirectionalLight::setSpecular(specular);
        setDirection(direction);
        specularPower = specular_power;
    }

    void setAmbient(const glm::vec3& ambient) override
    {
        this->ambient = ambient;
    }

    void setDiffuse(const glm::vec3& diffuse) override
    {
        this->diffuse = diffuse;
    }

    void setSpecular(const glm::vec3& specular) override
    {
        this->specular = specular;
    }

    void setDirection(const glm::vec3& direction)
    {
        this->direction = direction;
    }

    [[nodiscard]] glm::vec3 getAmbient() const override
    {
        return ambient;
    }

    [[nodiscard]] glm::vec3 getDiffuse() const override
    {
        return diffuse;
    }

    [[nodiscard]] glm::vec3 getSpecular() const override
    {
        return specular;
    }

    [[nodiscard]] glm::vec3 getDirection() const
    {
        return direction;
    }

    [[nodiscard]] float getSpecularPower() const
    {
        return specularPower;
    }

private:
    glm::vec3 direction;
    float specularPower = 0.5f;
};


#endif //DIRECTIONALLIGHT_H
