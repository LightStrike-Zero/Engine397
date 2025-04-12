//
// Created by light on 12/04/2025.
//

#ifndef SKYBOXCOMPONENT_H
#define SKYBOXCOMPONENT_H
// SkyboxComponent.h
#include <cstdint>

/**
 * @struct SkyboxComponent
 * @brief Component used to mark an entity as a skybox.
 *
 * Contains the GPU handle for the cubemap texture.
 */
struct SkyboxComponent {
  uint32_t cubemapID = 0;
  std::string shaderID;                   ///< Identifier for the shader used to render the skybox

};

#endif //SKYBOXCOMPONENT_H
