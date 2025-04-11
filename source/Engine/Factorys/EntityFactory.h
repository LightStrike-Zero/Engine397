//
// Created by Shaun on 8/04/2025.
//

#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H


#include <string>
#include "Terrain/Terrain.h"

// Include your component headers
#include "Components/RenderableComponent.h"
#include "Components/TransformComponent.h"
#include "Components/MaterialComponent.h"
#include "Components/PlayerControllerComponent.h"

// Include model/mesh loading support (e.g., ModelLoader, AssimpImporter, etc.)
#include "Importers/ModelLoader.h"
#include "Importers/AssimpImporter.h"
#include "ResourceManagement/EnttFacade.h"

class EntityFactory {
public:
    // Construct the factory with a pointer to the EnttFacade.
    // The factory does not own the facade.
    EntityFactory(EnttFacade* enttFacade);

    // Create entities from a generic model file.
    void createEntitiesFromModel(const std::string& filepath);
    
    // Create entities for a player model by adding an additional PlayerControllerComponent.
    void createPlayerEntitiesFromModel(const std::string& filepath);

    // Create a terrain entity from Terrain data.
    void addTerrainEntity(const Terrain& terrain);

    // void createCollidableEntitiesFromModel(const std::string& filepath);
    void createCollidableBoxEntitiesFromModel(const std::string& filepath);
    void createCollidableCapsuleEntitiesFromModel(const std::string& filepath);
    void createCollidableSphereEntitiesFromModel(const std::string& filepath);

    
private:
    EnttFacade* m_entityFacade;
    static std::vector<glm::vec3> extractPositions(const std::vector<Vertex>& meshVertices) ;
};

#endif //ENTITYFACTORY_H
