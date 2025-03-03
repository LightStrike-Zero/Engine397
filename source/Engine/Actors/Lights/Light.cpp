// Light.cpp
#include "Light.h"

void Light::setAmbient(const glm::vec3& ambient) {
    this->ambient = ambient;
}

void Light::setDiffuse(const glm::vec3& diffuse) {
    this->diffuse = diffuse;
}

void Light::setSpecular(const glm::vec3& specular) {
    this->specular = specular;
}

glm::vec3 Light::getAmbient() const {
    return ambient;
}

glm::vec3 Light::getDiffuse() const {
    return diffuse;
}

glm::vec3 Light::getSpecular() const {
    return specular;
}
