//
// Created by Shaun on 8/04/2025.
//

#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <string>

#include "EnttFacade.h"
#include "Factorys/EntityFactory.h"
#include "Lights/DirectionalLight.h"
#include "Terrain/Terrain.h"

class Scene {
public:
    Scene();
    ~Scene();

    void initialize();

    void update(float deltaTime);

    void loadModelEntity(const std::string& modelFilePath);
    void loadPlayerModelEntity(const std::string& modelFilePath);
    void addTerrainEntity(const Terrain& terrain);

    EnttFacade& getEntityManager() { return m_entityFacade; }

    void setDirectionalLight(const DirectionalLight& light) { m_directionalLight = light; }
    const DirectionalLight& getDirectionalLight() const { return m_directionalLight; }


private:
    // The ECS world managed as a facade.
    EnttFacade m_entityFacade;
    // The factory tat loads models and creates entities.
    std::unique_ptr<EntityFactory> m_entityFactory;

    // Scene data: lighting, terrain, etc.
    DirectionalLight m_directionalLight = DirectionalLight(glm::vec3(0.5f, -1.0f, -0.5f), glm::vec3(0.09f, 0.09f, 0.1f),
        glm::vec3(0.79f, 0.79f, 0.85f), glm::vec3(0.39f, 0.39f, 0.45f), 0.5f);
};

#endif //SCENE_H
