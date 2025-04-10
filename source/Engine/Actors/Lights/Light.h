//
// Created by Shaun on 8/12/2024.
//

/**
 * @file Light.h
 * @brief Base class for different types of light sources.
 * Provides a common interface for ambient, diffuse, and specular light components.
 * @author Shaun
 * @date 2024-12-08
 */

#ifndef LIGHT_H
#define LIGHT_H

#include <glm/vec3.hpp>

/**
 * @class Light
 * @brief Abstract base class representing a light source.
 *
 * This class defines lighting parameters: ambient, diffuse and specular
 */
class Light
{
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~Light() = default;

    /**
     * @brief Set the ambient colour of the light.
     * @param ambient The ambient colour vector (vec3).
     */
    virtual void setAmbient(const glm::vec3& ambient);

    /**
     * @brief Set the diffuse colour of the light.
     * @param diffuse The diffuse colour vector (vec3).
     */
    virtual void setDiffuse(const glm::vec3& diffuse);

    /**
     * @brief Set the specular colour of the light.
     * @param specular The specular colour vector (vec3).
     */
    virtual void setSpecular(const glm::vec3& specular);

    /**
     * @brief Get the ambient colour of the light.
     * @return The ambient colour vector (vec3).
     */
    [[nodiscard]] virtual glm::vec3 getAmbient() const;

    /**
     * @brief Get the diffuse colour of the light.
     * @return The diffuse colour vector (vec3).
     */
    [[nodiscard]] virtual glm::vec3 getDiffuse() const;
    [[nodiscard]] virtual glm::vec3 getSpecular() const;

protected:
    glm::vec3 ambient{};         ///< Ambient colour component
    glm::vec3 diffuse{};         ///< Diffuse colour component
    glm::vec3 specular{};        ///< Specular colour component
};

#endif //LIGHT_H
