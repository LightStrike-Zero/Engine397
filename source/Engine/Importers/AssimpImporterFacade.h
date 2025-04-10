//
// Created by Shaun on 11/04/2025.
//

#ifndef ASSIMPIMPORTERFACADE_H
#define ASSIMPIMPORTERFACADE_H

#include <string>
#include <vector>

#include "Components/DataTypes.h"         
#include "Components/RenderableComponent.h"

class AssimpImporterFacade {
public:
    AssimpImporterFacade();
    ~AssimpImporterFacade();

    // Loads a model from the given filepath.
    // The caller gets a LoadedModel structure with only engine data types.
    bool loadModel(const std::string& filepath, std::vector<RawMeshData>& meshes);

    // Configures a RenderableComponent using loaded mesh data.
    //TODO still not sure about putting components in the same file as the importer
    void setupMesh(RenderableComponent& renderableComponent);

private:
    // Internal instance of the Assimp importer.
    class AssimpImporter* m_importer;  
};


#endif //ASSIMPIMPORTERFACADE_H
