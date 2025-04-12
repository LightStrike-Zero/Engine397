//
// Created by Shaun on 11/04/2025.
//
#include "AssimpImporterFacade.h"

#include <stdexcept>

#include "AssimpImporter.h"
#include <string>
#include <vector>

AssimpImporterFacade::AssimpImporterFacade()
{
    m_importer = new AssimpImporter();
}

AssimpImporterFacade::~AssimpImporterFacade()
{
    delete m_importer;
}

bool AssimpImporterFacade::loadModel(const std::string& filepath, std::vector<RawMeshData>& meshes)
{
    return m_importer->loadModel(filepath, meshes);
}

void AssimpImporterFacade::setupMesh(RenderableComponent& renderableComponent)
{
    m_importer->setupMesh(renderableComponent);
}
