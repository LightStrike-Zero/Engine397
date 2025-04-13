//
// Created by Shaun on 8/04/2025.
//

/**
 * @file Scene.h
 * @brief Represents a 3D scene containing terrain, entities, and lights.
 * @author Shaun
 * @date 2025-04-08
 */

#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <string>

#include "EnttFacade.h"
#include "Factorys/EntityFactory.h"
#include "Lights/DirectionalLight.h"
#include "Terrain/Terrain.h"

/**
 * @class Scene
 * @brief Represents a 3D scene containing terrain, entities, and lights.
 */
class Scene {
public:
    /**
     * @brief Constructs a new Scene instance.
     */
    Scene();

    /**
     * @brief Destroys the Scene.
     */
    ~Scene();

    /**
     * @brief Initializes the scene, entities, or components.
     */
    void initialize();

    /**
     * @brief Updates the scene.
     * @param deltaTime Time since the last update.
     */
    void update(float deltaTime);

    /**
     * @brief Loads a general model entity into the scene.
     * @param modelFilePath Path to the model file.
     */
    void loadModelEntity(const std::string& modelFilePath, const std::string& name);

    /**
     * @brief Loads the player model into the scene.
     * @param modelFilePath Path to the player model.
     */
    void loadPlayerModelEntity(const std::string& modelFilePath);

    /**
     * @brief Adds a terrain entity to the scene.
     * @param terrain Terrain data object.
     */
    void addTerrainEntity(const Terrain& terrain);

    /**
     * @deprecated USE box, capsule, or sphere instead.
     */
    // void loadCollidableEntity(const std::string& filepath);
    void loadCollidableBoxEntity(const std::string& filepath, const std::string& name);
    void loadCollidableCapsuleEntity(const std::string& filepath, const std::string& name);
    void loadCollidableSphereEntity(const std::string& filepath, const std::string& name);

    /**
     * @brief Returns access to the entity manager (EnTT facade).
     * @return Reference to the EnttFacade.
     */
    EnttFacade& getEntityManager() { return m_entityFacade; }

    /**
     * @brief Sets the scene's directional light.
     * @param light Directional light to set.
     */
    void setDirectionalLight(const DirectionalLight& light) { m_directionalLight = light; }

    /**
     * @brief Gets the current directional light in the scene.
     * @return Reference to the directional light.
     */
    const DirectionalLight& getDirectionalLight() const { return m_directionalLight; }

    bool Scene::setEntityPosByName(const std::string& name, float x, float y, float z); //not using glm::vec3 because lua cannot bind user type
  void createSkyBox(const std::array<std::string, 6>& faces);

private:

    EnttFacade m_entityFacade;                         // The ECS world managed as a facade.

    std::unique_ptr<EntityFactory> m_entityFactory;   // The factory tat loads models and creates entities.

        // Scene data: lighting, terrain, etc.
    DirectionalLight m_directionalLight = DirectionalLight(glm::vec3(0.5f, -1.0f, -0.5f), glm::vec3(0.09f, 0.09f, 0.1f),
        glm::vec3(0.79f, 0.79f, 0.85f), glm::vec3(0.39f, 0.39f, 0.45f), 0.5f);
};


#endif //SCENE_H
