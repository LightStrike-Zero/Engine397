/**
 * @file Material.h
 * @brief Implementation of the Material class which is responisble for loading and creating an OpenGL
 * material from a MTL file associated with an OBJ file.
 *
 * If no material is read, then a defualt material is returned so that the program does not crash
 * and visually alerts developer to an issue
 *
 *
 * @author Shaun Matthews
 * @date Created: 5/09/2024
 * @date Modified:
 */

#ifndef MATERIAL_H
#define MATERIAL_H

#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>

/**
 * @class Material
 * @brief A class for managing material properties used in OpenGL rendering.
 *
 * The Material class is responsible for loading material properties from a .mtl file and applying them in
 * OpenGL's fixed-function pipeline. The material includes properties such as shininess, ambient, diffuse,
 * specular, and emissive colors, as well as opacity (for blending). If the material fails to load, a default
 * material is used to prevent rendering errors and alert the developer.
 */
class Material
{
public:
    // Constructors
    Material();
    Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
             const glm::vec3& emissive, GLfloat shininess, GLfloat opacity);

    // Static method to load from MTL file
    static Material loadFromMTL(const std::string& mtlFile);

    // Apply material
    void apply() const;

    // Getters
    glm::vec3 getAmbient() const;
    glm::vec3 getDiffuse() const;
    glm::vec3 getSpecular() const;
    glm::vec3 getEmissive() const;
    GLfloat getShininess() const;
    GLfloat getOpacity() const;

    // Setters
    void setAmbient(const glm::vec3& ambient);
    void setDiffuse(const glm::vec3& diffuse);
    void setSpecular(const glm::vec3& specular);
    void setEmissive(const glm::vec3& emissive);
    void setShininess(GLfloat shininess);
    void setOpacity(GLfloat opacity);

private:
    // Material properties
    glm::vec3 Ka; ///< Ambient color
    glm::vec3 Kd; ///< Diffuse color
    glm::vec3 Ks; ///< Specular color
    glm::vec3 Ke; ///< Emissive color
    GLfloat Ns; ///< Shininess
    GLfloat d; ///< Opacity
};

#endif // MATERIAL_H
