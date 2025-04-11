//
// Created by Shaun on 29/11/2024.
//

/**
 * @file ModelLoader.h
 * @brief Provides a singleton interface for loading 3D model data via AssimpImporter.
 * Includes a struct and a class
 * @author Shaun
 * @date 2024-11-29
 */

#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <string>

#include "AssimpImporter.h"
#include "AssimpImporterFacade.h"

/**
 * @struct LoadedModel
 * @brief Contains raw mesh data loaded from a model file.
 */
struct LoadedModel {
    std::vector<RawMeshData> meshes; 
};

/**
 * @class ModelLoader
 * @brief Singleton class that wraps AssimpImporter and exposes a simple model loading interface.
 */
class ModelLoader {
public:
    /**
     * @brief Returns the singleton instance.
     */
    static ModelLoader& getInstance();

    /**
     * @brief Loads a model from a file and returns raw mesh data.
     * @param filepath Path to the model file.
     * @return A LoadedModel containing mesh data.
     */
    LoadedModel loadModel(const std::string& filepath);

private:
    ModelLoader() = default;
    ~ModelLoader() = default;

    ModelLoader(const ModelLoader&) = delete;
    ModelLoader& operator=(const ModelLoader&) = delete;

    AssimpImporterFacade m_importer;
};

#endif //MODELLOADER_H
