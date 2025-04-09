//
// Created by Shaun on 8/04/2025.
//

#ifndef NEWSCENE_H
#define NEWSCENE_H

#include <memory>
#include <string>
#include <vector>

// Include your abstract ECS interface.
// Include your factory (or resource manager) for model-based entity creation.
// #include "EntityFactory.h"

// Optional: Include other scene-specific classes.
// For example, for lights, terrain, or camera handling.

#include <memory>
#include <string>
#include <vector>

// Include the facade directly.
#include "EnttFacade.h"
// Include the factory.

// Include scene-specific classes (lights, terrain, etc.)
#include "Factorys/EntityFactory.h"
#include "Lights/DirectionalLight.h"
#include "Terrain/Terrain.h"

class Scene {
public:
    // Constructor: Scene will internally create the ECS facade and the factory.
    Scene();
    ~Scene();

    // Initialize the scene (load static resources, create initial entities, etc.)
    void initialize();

    // Update the scene state each frame.
    void update(float deltaTime);

    // Delegate model/entity creation to the entity factory.
    void loadModelEntity(const std::string& modelFilePath);
    void loadPlayerModelEntity(const std::string& modelFilePath);
    void addTerrainEntity(const Terrain& terrain);

    // Accessor for the ECS system (facade).
    EnttFacade& getEntityManager() { return m_entityFacade; }

    // Scene-specific resource management.
    void setDirectionalLight(const DirectionalLight& light) { m_directionalLight = light; }
    const DirectionalLight& getDirectionalLight() const { return m_directionalLight; }

    // void addTerrain(const Terrain& terrain) { m_terrains.push_back(terrain); }
    // const std::vector<Terrain>& getTerrains() const { return m_terrains; }

private:
    // The ECS world managed as a facade.
    EnttFacade m_entityFacade;

    // The factory that loads models and creates entities.
    std::unique_ptr<EntityFactory> m_entityFactory;

    // Scene data: lighting, terrain, etc.
    DirectionalLight m_directionalLight = DirectionalLight(glm::vec3(0.5f, -1.0f, -0.5f), glm::vec3(0.09f, 0.09f, 0.1f),
        glm::vec3(0.79f, 0.79f, 0.85f), glm::vec3(0.39f, 0.39f, 0.45f), 0.5f);
    // std::vector<Terrain> m_terrains;
};

#endif //NEWSCENE_H
