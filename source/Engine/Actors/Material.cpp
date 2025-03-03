/**
 * @file Material.cpp
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

#include "Material.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

/**
  * @brief Default constructor initializes material properties with default values.
  *
  * This constructor sets up a default material with white ambient and diffuse colors,
  * gray specular color, and no emissive color. The opacity is set to fully opaque.
  */
/**
 * @brief Default constructor initializes material properties with default values.
 */
Material::Material() : Ns(32.0f), d(1.0f),
    Ka(glm::vec3(1.0f, 1.0f, 1.0f)),
    Kd(glm::vec3(1.0f, 1.0f, 1.0f)),
    Ks(glm::vec3(1.0f, 1.0f, 1.0f)),
    Ke(glm::vec3(0.0f, 0.0f, 0.0f))
{
    // Default values are set using initializer list
}

/**
 * @brief Parameterized constructor to manually set material properties.
 */
Material::Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
                 const glm::vec3& emissive, GLfloat shininess, GLfloat opacity)
    : Ka(ambient), Kd(diffuse), Ks(specular), Ke(emissive), Ns(shininess), d(opacity)
{
}

/**
 * @brief Static method to load a material from a .mtl file.
 */
Material Material::loadFromMTL(const std::string& mtlFile) {
    std::ifstream file(mtlFile);
    Material material; // Starts with default values

    if (!file.is_open()) {
        std::cerr << "Failed to open MTL file: " << mtlFile << std::endl;
        return material; // Return default material if file can't be opened
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        // Parsing material properties
        if (prefix == "Ns") {
            iss >> material.Ns; // Shininess
        }
        else if (prefix == "Ka") {
            GLfloat r, g, b;
            iss >> r >> g >> b;
            material.Ka = glm::vec3(r, g, b); // Ambient color
        }
        else if (prefix == "Kd") {
            GLfloat r, g, b;
            iss >> r >> g >> b;
            material.Kd = glm::vec3(r, g, b); // Diffuse color
        }
        else if (prefix == "Ks") {
            GLfloat r, g, b;
            iss >> r >> g >> b;
            material.Ks = glm::vec3(r, g, b); // Specular color
        }
        else if (prefix == "Ke") {
            GLfloat r, g, b;
            iss >> r >> g >> b;
            material.Ke = glm::vec3(r, g, b); // Emissive color
        }
        else if (prefix == "d" || prefix == "Tr") { // 'Tr' is the inverse of 'd' in some MTL files
            iss >> material.d; // Opacity
        }
    }

    file.close();
    return material;
}

/**
 * @brief Applies the material properties in OpenGL using the fixed-function pipeline.
 */
void Material::apply() const
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, glm::value_ptr(Ka));
    glMaterialfv(GL_FRONT, GL_DIFFUSE, glm::value_ptr(Kd));
    glMaterialfv(GL_FRONT, GL_SPECULAR, glm::value_ptr(Ks));
    glMaterialfv(GL_FRONT, GL_EMISSION, glm::value_ptr(Ke));
    glMaterialf(GL_FRONT, GL_SHININESS, Ns);
}

/**
 * @brief Gets the ambient color.
 */
glm::vec3 Material::getAmbient() const {
    return Ka;
}

/**
 * @brief Gets the diffuse color.
 */
glm::vec3 Material::getDiffuse() const {
    return Kd;
}

/**
 * @brief Gets the specular color.
 */
glm::vec3 Material::getSpecular() const {
    return Ks;
}

/**
 * @brief Gets the emissive color.
 */
glm::vec3 Material::getEmissive() const {
    return Ke;
}

/**
 * @brief Gets the shininess.
 */
GLfloat Material::getShininess() const {
    return Ns;
}

/**
 * @brief Gets the opacity.
 */
GLfloat Material::getOpacity() const {
    return d;
}

/**
 * @brief Sets the ambient color.
 */
void Material::setAmbient(const glm::vec3& ambient) {
    Ka = ambient;
}

/**
 * @brief Sets the diffuse color.
 */
void Material::setDiffuse(const glm::vec3& diffuse) {
    Kd = diffuse;
}

/**
 * @brief Sets the specular color.
 */
void Material::setSpecular(const glm::vec3& specular) {
    Ks = specular;
}

/**
 * @brief Sets the emissive color.
 */
void Material::setEmissive(const glm::vec3& emissive) {
    Ke = emissive;
}

/**
 * @brief Sets the shininess.
 */
void Material::setShininess(GLfloat shininess) {
    Ns = shininess;
}

/**
 * @brief Sets the opacity.
 */
void Material::setOpacity(GLfloat opacity) {
    d = opacity;
}