//
// Created by Shaun on 1/12/2024.
//

// TextureLoader.cpp
#include "TextureLoader.h"

#include <iostream>

#include "Factorys/TextureFactory.h"
#include "OpenGL/OGL_CubeMap.h"

std::shared_ptr<ITexture> TextureLoader::loadTexture(const std::string &filePath) {
  return TextureFactory::createTexture(filePath);
}

std::shared_ptr<ITexture> TextureLoader::loadFromGLTF(const std::string &gltfTexturePath) {
  return TextureFactory::createTexture(gltfTexturePath);
}

std::shared_ptr<ITexture> TextureLoader::loadFromMemory(const RawImageData &imageData) {
  return TextureFactory::createTexture(imageData);
}
std::shared_ptr<ICubeMap> TextureLoader::loadCubeMap(const std::array<std::string, 6>& faces) {
  return TextureFactory::createTexture(faces);
}