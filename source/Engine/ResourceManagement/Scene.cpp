//
// Created by Shaun on 8/04/2025.
//

#include "Scene.h"

#include <iostream>

Scene::Scene()
    : m_entityFactory(std::make_unique<EntityFactory>(&m_entityFacade)), m_entityBuilder(std::make_unique<EntityBuilder>(&m_entityFacade))
{
}

Scene::~Scene()
{
    //TODO need to add clean up and TEST
}

void Scene::initialize()
{
    // Set initial scene state
}

void Scene::update(float deltaTime)
{
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

void Scene::addWaterEntity(const Water &water) {
    if(m_entityFactory) {
        m_entityFactory->addWaterEntity(water);
    }
}

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


void Scene::createSkyBox(const std::array<std::string, 6> &faces) {
    m_entityFactory->createSkyBox(faces);
}


bool Scene::setEntityPosByName(const std::string& name, float x, float y, float z) { //not using glm::vec3 because lua cannot bind user type
    auto view = m_entityFacade.getRegistry().view<NameComponent,TransformComponent>();
    //essentially grabbing everything else in a transform, because we only want to change position, not others
    for (auto entity : view) {
        if (view.get<NameComponent>(entity).name == name) {
            TransformComponent transform = view.get<TransformComponent>(entity);
            transform.position = {x,y,z}; //the actual data we're modifying
            m_entityFacade.setComponentByName(name, transform);
            std::cout << "i am called and used!" << std::endl;
            return true;
        }
    }
    return false;
}

EntityBuilder Scene::createEntity()
{
    //TODO test that this doesnt expose the facade address 
    return EntityBuilder(&m_entityFacade);
}

entt::entity Scene::createNPC(const std::string& modelPath, const std::string& npcType, const glm::vec3& position)
{
    return EntityBuilder(&m_entityFacade)
        .createEntity()
        .fromModel(modelPath)
        .withPosition(position)
        .asNPC(npcType)
        .withBoxCollider()
        .build();
}

entt::entity Scene::createPlayer(const std::string& modelPath, int playerID,  const glm::vec3& position) {
    return EntityBuilder(&m_entityFacade)
        .createEntity()
        .fromModel(modelPath)
        .withPosition(position)
        .asPlayer(playerID)
        .withCapsuleCollider()
        .build();
}

