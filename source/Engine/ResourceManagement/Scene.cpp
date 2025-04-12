//
// Created by Shaun on 8/04/2025.
//

#include "Scene.h"

Scene::Scene()
    : m_entityFactory(std::make_unique<EntityFactory>(&m_entityFacade))
{
}

Scene::~Scene() {
}

void Scene::initialize() {
    // Set initial scene state
}

void Scene::update(float deltaTime) {
    // Update
}

void Scene::loadModelEntity(const std::string& modelFilePath, const std::string& name) {
    if (m_entityFactory)
        m_entityFactory->createEntitiesFromModel(modelFilePath, name);
}

void Scene::loadPlayerModelEntity(const std::string& modelFilePath) {
    if (m_entityFactory)
        m_entityFactory->createPlayerEntitiesFromModel(modelFilePath);
}

void Scene::addTerrainEntity(const Terrain& terrain) {
    if (m_entityFactory)
        m_entityFactory->addTerrainEntity(terrain);
}

// void Scene::loadCollidableEntity(const std::string& filepath) {
//     if (m_entityFactory)
//         m_entityFactory->createCollidableEntitiesFromModel(filepath);
//     }

void Scene::loadCollidableBoxEntity(const std::string& filepath, const std::string& name) {
    if (m_entityFactory)
        m_entityFactory->createCollidableBoxEntitiesFromModel(filepath, name);
}

void Scene::loadCollidableCapsuleEntity(const std::string& filepath, const std::string& name) {
    if (m_entityFactory)
        m_entityFactory->createCollidableCapsuleEntitiesFromModel(filepath, name);
}

void Scene::loadCollidableSphereEntity(const std::string& filepath, const std::string& name) {
    if (m_entityFactory)
        m_entityFactory->createCollidableSphereEntitiesFromModel(filepath, name);

}
