//
// Created by Shaun on 11/02/2025.
//

/**
 * @file DierctionalLight.h
 * @brief Defines the DierctionalLight class used to represent a directional light source.
 * @author Shaun
 * @date 2025-02-11
 */

#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include <glm/vec3.hpp>

#include "Light.h"

/**
 * @class DirectionalLight
 * @brief A class representing a directional light source.
 * 
 * This class inherits from the Light class and provides additional
 * functionality specific to directional lights, such as setting and
 * getting the direction of the light.
 */
class DirectionalLight : public Light
{
public:
    /**
    * @brief Constructor for DirectionalLight.
    * 
    * @param direction The direction of the light.
    * @param ambient The ambient colour of the light.
    * @param diffuse The diffuse colour of the light.
    * @param specular The specular colour of the light.
    * @param specular_power The specular power of the light.
    */
    DirectionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse,
                     const glm::vec3& specular, const float specular_power)

    {
        DirectionalLight::setAmbient(ambient);
        DirectionalLight::setDiffuse(diffuse);
        DirectionalLight::setSpecular(specular);
        setDirection(direction);
        specularPower = specular_power;
    }

    /// @copydoc Light::setAmbient
    void setAmbient(const glm::vec3& ambient) override
    {
        this->ambient = ambient;
    }

    /// @copydoc Light::setDiffuse
    void setDiffuse(const glm::vec3& diffuse) override
    {
        this->diffuse = diffuse;
    }

    /// @copydoc Light::setSpecular
    void setSpecular(const glm::vec3& specular) override
    {
        this->specular = specular;
    }

    /**
     * @brief Set the direction of the directional light.
     * @param direction Normalised direction vector.
     */
    void setDirection(const glm::vec3& direction)
    {
        this->direction = direction;
    }

    /**
     * @brief Get the direction of the light.
     * @return Direction vector.
     */
    [[nodiscard]] glm::vec3 getAmbient() const override
    {
        return ambient;
    }

    /// @copydoc Light::getAmbient
    [[nodiscard]] glm::vec3 getDiffuse() const override
    {
        return diffuse;
    }
    /// @copydoc Light::getDiffuse
    [[nodiscard]] glm::vec3 getSpecular() const override
    {
        return specular;
    }

    /// @copydoc Light::getSpecular
    [[nodiscard]] glm::vec3 getDirection() const
    {
        return direction;
    }

    /**
     * @brief Get the specular power of the light.
     * @return Float value representing the specular power.
     */
    [[nodiscard]] float getSpecularPower() const
    {
        return specularPower;
    }

private:
    glm::vec3 direction;               ///< Direction the light is coming from
    float specularPower = 0.5f;        ///< Strength of the specular reflection
};

#endif //DIRECTIONALLIGHT_H
