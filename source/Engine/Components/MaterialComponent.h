//
// Created by Shaun on 28/12/2024.
//

#ifndef MATERIALCOMPONENT_H
#define MATERIALCOMPONENT_H

#include <GL/glew.h>

#include "MaterialData.h"

class Texture;

struct MaterialComponent
{
    std::string shaderID;
    GLuint baseColorTextureID = 0; // Albedo texture
    GLuint metalnessTextureID = 0; // Metalness texture
    GLuint roughnessTextureID = 0; // Roughness texture
    GLuint normalTextureID = 0; // Normal map

    bool isDecal = false;

    MaterialComponent() = default;

    // Constructor to initialize from RawMaterialData
    MaterialComponent(const RawMaterialData& materialData, const std::string& inShaderID)
        :   shaderID(inShaderID),
            baseColorTextureID(materialData.baseColorTextureID),
            metalnessTextureID(materialData.metalnessTextureID),
            roughnessTextureID(materialData.roughnessTextureID),
            normalTextureID(materialData.normalTextureID),
            isDecal(materialData.isDecal)
    {
    }
};


#endif //MATERIALCOMPONENT_H
