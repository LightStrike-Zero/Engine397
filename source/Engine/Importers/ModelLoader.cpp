//
// Created by Shaun on 29/11/2024.
//

#include "ModelLoader.h"

#include <iostream>
#include <ostream>

#include "AssimpImporter.h"

ModelLoader& ModelLoader::getInstance() {
    static ModelLoader instance;
    return instance;
}

LoadedModel ModelLoader::loadModel(const std::string& filepath) {
    AssimpImporter importer;
    LoadedModel loadedModel;

    // Load raw mesh and material data
    if (importer.loadModel(filepath, loadedModel.meshes/*, loadedModel.materials*/)) {
    } else {
        std::cerr << "Failed to load model: " << filepath << std::endl;
    }

    // Return raw model data
    return loadedModel;
}