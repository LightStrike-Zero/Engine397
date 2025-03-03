//
// Created by Shaun on 11/02/2025.
//

#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include <glm/vec3.hpp>

#include "Light.h"


class PointLight : public Light
{
public:
    PointLight(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
        : position(position)

    {
        PointLight::setAmbient(ambient);
        PointLight::setDiffuse(diffuse);
        PointLight::setSpecular(specular);
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

    [[nodiscard]] glm::vec3 getPosition() const
    {
        return position;
    }

    [[nodiscard]] float getConstant() const
    {
        return constant;
    }

    [[nodiscard]] float getLinear() const
    {
        return linear;
    }

    [[nodiscard]] float getQuadratic() const
    {
        return quadratic;
    }

private:
    glm::vec3 position;                 // Position of the light
    float constant = 1.0f;              // Constant attenuation
    float linear = 0.09f;               // Linear attenuation
    float quadratic = 0.032f;           // Quadratic attenuation
};



#endif //POINTLIGHT_H
