//
// Created by Shaun on 8/04/2025.
//

#include "EntityFactory.h"

#include "Components/CollisionComponents/CollidableComponent.h"
#include "OpenGL/OpenGLMeshBuffer.h"

EntityFactory::EntityFactory(EnttFacade* enttFacade)
    : m_entityFacade(enttFacade)
{
}

void EntityFactory::createEntitiesFromModel(const std::string& filepath) {
    // Get the model loader (singleton or instance as needed)
    ModelLoader& loader = ModelLoader::getInstance();
    
    // Load model data from file.
    LoadedModel modelData = loader.loadModel(filepath);
    AssimpImporter importer;
    
    // For every mesh in the model data, create an entity and attach components.
    for (const auto& rawMesh : modelData.meshes) {
        // Create an entity.
        auto entity = m_entityFacade->createEntity();

        // Create and set up the renderable component.
        RenderableComponent meshComponent(rawMesh);
        importer.setupMesh(meshComponent);
        m_entityFacade->addComponent<RenderableComponent>(entity, meshComponent);

        // Create and set up the transform component.
        TransformComponent transformComponent;
        transformComponent.setFromModelMatrix(rawMesh.transform);
        m_entityFacade->addComponent<TransformComponent>(entity, transformComponent);

        // Create the material component (shader name is hard-coded for now).
        MaterialComponent materialComponent(rawMesh.material, "lightingShader");
        m_entityFacade->addComponent<MaterialComponent>(entity, materialComponent);
    }
}

void EntityFactory::createCollidableEntitiesFromModel(const std::string& filepath) {
    ModelLoader& loader = ModelLoader::getInstance();

    // Load the raw model data
    LoadedModel modelData = loader.loadModel(filepath);

    // Process raw meshes into MeshComponents
    AssimpImporter importer;
    for (const auto& rawMesh : modelData.meshes)
    {
        entt::entity entity = m_entityFacade->createEntity();

        RenderableComponent meshComponent(rawMesh);
        importer.setupMesh(meshComponent);
        m_entityFacade->addComponent<RenderableComponent>(entity, meshComponent);

        TransformComponent transformComponent;
        transformComponent.setFromModelMatrix(rawMesh.transform);
        m_entityFacade->addComponent<TransformComponent>(entity, transformComponent);

        // for now im hard-coding the lighting shader into this, but it needs a way of being dynamically set
        MaterialComponent materialComponent(rawMesh.material, "lightingShader");
        m_entityFacade->addComponent<MaterialComponent>(entity, materialComponent);

        CollidableComponent collidableComponent{1};
        m_entityFacade->addComponent<CollidableComponent>(entity, collidableComponent);
    }
}

void EntityFactory::createPlayerEntitiesFromModel(const std::string& filepath) {
    ModelLoader& loader = ModelLoader::getInstance();
    LoadedModel modelData = loader.loadModel(filepath);
    AssimpImporter importer;

    for (const auto& rawMesh : modelData.meshes) {
        auto entity = m_entityFacade->createEntity();

        RenderableComponent meshComponent(rawMesh);
        importer.setupMesh(meshComponent);
        m_entityFacade->addComponent<RenderableComponent>(entity, meshComponent);

        TransformComponent transformComponent;
        transformComponent.setFromModelMatrix(rawMesh.transform);
        m_entityFacade->addComponent<TransformComponent>(entity, transformComponent);

        MaterialComponent materialComponent(rawMesh.material, "lightingShader");
        m_entityFacade->addComponent<MaterialComponent>(entity, materialComponent);

        // Add a component specific to player-controlled entities.
        PlayerControllerComponent controllerComponent{1};
        m_entityFacade->addComponent<PlayerControllerComponent>(entity, controllerComponent);
    }
}


void EntityFactory::addTerrainEntity(const Terrain& terrain) {
    // Create a renderable component from the terrain's mesh data.
    RenderableComponent terrainComponent(terrain.getMeshData());
    terrainComponent.meshBuffer = std::make_shared<OpenGLMeshBuffer>(
        terrainComponent.indices.size(),
        terrainComponent.vertices.size(),
        terrainComponent.indices.data(),
        terrainComponent.vertices.data()
    );
    
    auto entity = m_entityFacade->createEntity();
    m_entityFacade->addComponent<RenderableComponent>(entity, terrainComponent);

    TransformComponent transformComponent;
    transformComponent.setFromModelMatrix(terrain.getMeshData().transform);
    m_entityFacade->addComponent<TransformComponent>(entity, transformComponent);

    MaterialComponent materialComponent(terrain.getMeshData().material, "lightingShader");
    m_entityFacade->addComponent<MaterialComponent>(entity, materialComponent);
}