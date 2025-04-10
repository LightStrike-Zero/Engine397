//
// Created by Shaun on 29/11/2024.
//

#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <string>

#include "AssimpImporter.h"
#include "AssimpImporterFacade.h"

struct LoadedModel {
    std::vector<RawMeshData> meshes; 
};

class ModelLoader {
public:
    static ModelLoader& getInstance();

    // Load a model and return its raw data
    LoadedModel loadModel(const std::string& filepath);

private:
    ModelLoader() = default;
    ~ModelLoader() = default;

    ModelLoader(const ModelLoader&) = delete;
    ModelLoader& operator=(const ModelLoader&) = delete;

    AssimpImporterFacade m_importer;
};

#endif //MODELLOADER_H
