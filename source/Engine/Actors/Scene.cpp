#include "Scene.h"


#include "Components/MaterialComponent.h"
#include "Components/PlayerControllerComponent.h"
#include "Components/RenderableComponent.h"
#include "Components/TransformComponent.h"
#include "Components/CollidableComponent.h"
#include "Importers/ModelLoader.h"
#include "OpenGL/OpenGLMeshBuffer.h"


//TODO This really should be more like a interface to Entt, we should have a separate scene management file
// this isn't really managing the scene, its just a collection of entities

class ModelLoader;
Scene::Scene() = default;

Scene::~Scene() = default;

entt::entity Scene::createEntity(const std::string& name)
{
    auto entity = m_registry.create();
    m_registry.emplace<std::string>(entity, name.empty() ? "Entity" : name);
    return entity;
}

void Scene::destroyEntity(entt::entity entity)
{
    m_registry.destroy(entity);
}


void Scene::loadModelToRegistry(const std::string& filepath) {
    // std::cout << "Registry address inside Scene: " << &m_registry << std::endl;
    ModelLoader& loader = ModelLoader::getInstance();

    // Load the raw model data
    LoadedModel modelData = loader.loadModel(filepath);

    // Process raw meshes into MeshComponents
    AssimpImporter importer;
    for (const auto& rawMesh : modelData.meshes) {
        entt::entity entity = m_registry.create();

        RenderableComponent meshComponent(rawMesh);
        importer.setupMesh(meshComponent);
        m_registry.emplace<RenderableComponent>(entity, meshComponent);

        TransformComponent transformComponent;
        transformComponent.setFromModelMatrix(rawMesh.transform);
        m_registry.emplace<TransformComponent>(entity, transformComponent);

        // for now im hard-coding the lighting shader into this, but it needs a way of being dynamically set
        MaterialComponent materialComponent(rawMesh.material, "lightingShader");
        m_registry.emplace<MaterialComponent>(entity, materialComponent);
    }
}

void Scene::loadPlayerModelToRegistry(const std::string& filepath) {
    // std::cout << "Registry address inside Scene: " << &m_registry << std::endl;
    ModelLoader& loader = ModelLoader::getInstance();

    // Load the raw model data
    LoadedModel modelData = loader.loadModel(filepath);

    // Process raw meshes into MeshComponents
    AssimpImporter importer;
    for (const auto& rawMesh : modelData.meshes) {
        entt::entity entity = m_registry.create();

        RenderableComponent meshComponent(rawMesh);
        importer.setupMesh(meshComponent);
        m_registry.emplace<RenderableComponent>(entity, meshComponent);

        TransformComponent transformComponent;
        transformComponent.setFromModelMatrix(rawMesh.transform);
        m_registry.emplace<TransformComponent>(entity, transformComponent);

        // for now im hard-coding the lighting shader into this, but it needs a way of being dynamically set
        MaterialComponent materialComponent(rawMesh.material, "lightingShader");
        m_registry.emplace<MaterialComponent>(entity, materialComponent);

        PlayerControllerComponent controllerComponent{1};
        m_registry.emplace<PlayerControllerComponent>(entity, controllerComponent);
    }
}

void Scene::loadCollidableModelToRegistry(const std::string& filepath) {
    // std::cout << "Registry address inside Scene: " << &m_registry << std::endl;
    ModelLoader& loader = ModelLoader::getInstance();

    // Load the raw model data
    LoadedModel modelData = loader.loadModel(filepath);

    // Process raw meshes into MeshComponents
    AssimpImporter importer;
    for (const auto& rawMesh : modelData.meshes) {
        entt::entity entity = m_registry.create();

        RenderableComponent meshComponent(rawMesh);
        importer.setupMesh(meshComponent);
        m_registry.emplace<RenderableComponent>(entity, meshComponent);

        TransformComponent transformComponent;
        transformComponent.setFromModelMatrix(rawMesh.transform);
        m_registry.emplace<TransformComponent>(entity, transformComponent);

        // for now im hard-coding the lighting shader into this, but it needs a way of being dynamically set
        MaterialComponent materialComponent(rawMesh.material, "lightingShader");
        m_registry.emplace<MaterialComponent>(entity, materialComponent);

        CollidableComponent collidableComponent{1};
        m_registry.emplace<CollidableComponent>(entity, collidableComponent);
    }
}


void Scene::addTerrainToScene(const Terrain &terrain) {
    RenderableComponent terrainComponent(terrain.getMeshData());
    
    terrainComponent.meshBuffer = std::make_shared<OpenGLMeshBuffer>(
        terrainComponent.indices.size(),
        terrainComponent.vertices.size(),
        terrainComponent.indices.data(),
        terrainComponent.vertices.data()
    );
    
    entt::entity entity = m_registry.create();
    m_registry.emplace<RenderableComponent>(entity, terrainComponent);
    
    TransformComponent transformComponent;
    transformComponent.setFromModelMatrix(terrain.getMeshData().transform);
    m_registry.emplace<TransformComponent>(entity, transformComponent);
    
    MaterialComponent materialComponent(terrain.getMeshData().material, "lightingShader");
    m_registry.emplace<MaterialComponent>(entity, materialComponent);
}