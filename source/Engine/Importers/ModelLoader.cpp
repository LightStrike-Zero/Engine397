//
// Created by Shaun on 29/11/2024.
//

#include "ModelLoader.h"

#include <iostream>
#include <ostream>

#include "AssimpImporter.h"

ModelLoader& ModelLoader::getInstance()
{
    static ModelLoader instance;
    return instance;
}


LoadedModel ModelLoader::loadModel(const std::string& filepath)
{
    LoadedModel model;
    // Load raw mesh and material data
    if (!m_importer.loadModel(filepath, model.meshes))
    {
        std::cerr << "Failed to load model: " << filepath << std::endl;
    }
    // Return raw model data
    return model;
}
