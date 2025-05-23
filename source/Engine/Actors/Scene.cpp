//
// Created by Shaun on 28/12/2024.
//

#include "Scene.h"

#include <iostream>

#include "Components/MaterialComponent.h"
#include "Components/MeshComponent.h"
#include "Components/TransformComponent.h"
#include "Importers/ModelLoader.h"

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


/*
 * This method should really be something to do with managing entities, maybe part of a wrapper??
 */
void Scene::loadModelToRegistry(const std::string& filepath) {
    // std::cout << "Registry address inside Scene: " << &m_registry << std::endl;
    ModelLoader& loader = ModelLoader::getInstance();

    // Load the raw model data
    LoadedModel modelData = loader.loadModel(filepath);

    // Process raw meshes into MeshComponents
    AssimpImporter importer;
    for (const auto& rawMesh : modelData.meshes) {
        entt::entity entity = m_registry.create();

        MeshComponent meshComponent(rawMesh);
        importer.setupMesh(meshComponent);
        m_registry.emplace<MeshComponent>(entity, meshComponent);

        TransformComponent transformComponent;
        transformComponent.setFromModelMatrix(rawMesh.transform);
        m_registry.emplace<TransformComponent>(entity, transformComponent);

        // for now im hard-coding the lighting shader into this, but it needs a way of being dynamically set
        MaterialComponent materialComponent(rawMesh.material, "lightingShader");
        m_registry.emplace<MaterialComponent>(entity, materialComponent);
    }
}