//
// Created by Shaun on 8/04/2025.
//

#include "NewScene.h"

NewScene::NewScene()
    : m_entityFactory(std::make_unique<EntityFactory>(&m_entityFacade))
{
    // Optionally, initialize default scene data here.
}

NewScene::~NewScene() {
    // Unique pointers will automatically clean up.
}

void NewScene::initialize() {
    // Set initial scene state and create static entities as needed.
    // For example, pre-load some models, set up lighting, etc.
}

void NewScene::update(float deltaTime) {
    // Update scene-wide logic, process input, and delegate ECS updates if needed.
}

void NewScene::loadModelEntity(const std::string& modelFilePath) {
    if (m_entityFactory)
        m_entityFactory->createEntitiesFromModel(modelFilePath);
}

void NewScene::loadPlayerModelEntity(const std::string& modelFilePath) {
    if (m_entityFactory)
        m_entityFactory->createPlayerEntitiesFromModel(modelFilePath);
}

void NewScene::addTerrainEntity(const Terrain& terrain) {
    if (m_entityFactory)
        m_entityFactory->addTerrainEntity(terrain);
}