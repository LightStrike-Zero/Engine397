//
// Created by Shaun on 8/04/2025.
//

#include "Scene.h"

Scene::Scene()
    : m_entityFactory(std::make_unique<EntityFactory>(&m_entityFacade))
{
    // Optionally, initialize default scene data here.
}

Scene::~Scene() {
    // Unique pointers will automatically clean up.
}

void Scene::initialize() {
    // Set initial scene state and create static entities as needed.
    // For example, pre-load some models, set up lighting, etc.
}

void Scene::update(float deltaTime) {
    // Update scene-wide logic, process input, and delegate ECS updates if needed.
}

void Scene::loadModelEntity(const std::string& modelFilePath) {
    if (m_entityFactory)
        m_entityFactory->createEntitiesFromModel(modelFilePath);
}

void Scene::loadPlayerModelEntity(const std::string& modelFilePath) {
    if (m_entityFactory)
        m_entityFactory->createPlayerEntitiesFromModel(modelFilePath);
}

void Scene::addTerrainEntity(const Terrain& terrain) {
    if (m_entityFactory)
        m_entityFactory->addTerrainEntity(terrain);
}