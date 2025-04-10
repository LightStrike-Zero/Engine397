//
// Created by Shaun on 28/12/2024.
//

/**
 * @file MaterialComponent.h
 * @brief Defines the struct Material Component that contains PBR texture IDs and shader reference for rendering.
 * @author Shaun
 * @date 2024-12-28
 */

#ifndef MATERIALCOMPONENT_H
#define MATERIALCOMPONENT_H

#include <GL/glew.h>

#include "Components/DataTypes.h"

class Texture;

/**
 * @struct MaterialComponent
 * @brief Contains PBR texture IDs and shader reference for rendering.
 */
struct MaterialComponent
{
    std::string shaderID;                   ///< Identifier for the shader used to render the material
    GLuint baseColorTextureID = 0;          ///< OpenGL texture ID for the albedo/base color map
    GLuint metalnessTextureID = 0;          ///< OpenGL texture ID for the metalness map
    GLuint roughnessTextureID = 0;          ///< OpenGL texture ID for the roughness map
    GLuint normalTextureID = 0;             ///< OpenGL texture ID for the normal map

    bool isDecal = false;                   ///< Whether this material should be rendered as a decal

    MaterialComponent() = default;

    /**
     * @brief Constructs the material component using data from raw material and shader ID.
     * @param materialData Raw material data containing texture IDs
     * @param inShaderID Name or ID of the shader used
     */
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
