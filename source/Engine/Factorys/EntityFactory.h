//
// Created by Shaun on 8/04/2025.
//

/**
 * @file EntityFactory.h
 * @brief Provides methods to create various types of entities in the scene.
 *
 * This class acts as a utility to create entities from model files, terrain, or for special-purpose roles
 * like the player. It uses an externally provided EnttFacade to register components and manage the entity registry.
 * @author Shaun
 * @date 2025-04-08
 */

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
#include "Water/Water.h"

/**
 * @class EntityFactory
 * @brief Creates and registers entities using the EnttFacade.
 */
class EntityFactory {
public:
    /**
     * @brief Construct the factory with a pointer to the EnttFacade. The factory does not own the facade.
     * @param enttFacade A pointer to the facade managing the entity registry.
     */
    EntityFactory(EnttFacade* enttFacade);

    /**
     * @brief Create entities from a generic model file.
     * @param filepath Path to the model file (e.g., .gltf, .obj).
     * @param name manually set the Name Component of the entity
     */
    void createEntitiesFromModel(const std::string& filepath, const std::string& name);

    /**
     * @brief Create entities for a player model by adding an additional PlayerControllerComponent.
     * @param filepath Path to the player model file.
     * @param name manually set the Name Component of the entity

     */
    void createPlayerEntitiesFromModel(const std::string& filepath);

    /**
     * @brief Create a terrain entity from Terrain data.
     * @param terrain Reference to the Terrain object.
     */
    void addTerrainEntity(const Terrain& terrain);

    void addWaterEntity(const Water& water);

    /**
     * @brief Adds collision-enabled entities based on a model file.
     * @param filepath Path to the model file.
     */


    void createCollidableBoxEntitiesFromModel(const std::string& filepath, const std::string& name);
    void createCollidableCapsuleEntitiesFromModel(const std::string& filepath, const std::string& name);
    void createCollidableSphereEntitiesFromModel(const std::string& filepath, const std::string& name);

    void createSkyBox(const std::array<std::string, 6>& faces);
private:
    EnttFacade* m_entityFacade;///< Pointer to the entity registry manager (not owned).
    static std::vector<glm::vec3> extractPositions(const std::vector<Vertex>& meshVertices) ;
};

#endif //ENTITYFACTORY_H
