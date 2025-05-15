//
// Created by Shaun on 8/04/2025.
//

#include "EntityFactory.h"

#include "Components/CollisionComponents/BoxColliderComponent.h"
#include "Components/CollisionComponents/CapsuleColliderComponent.h"
#include <iostream>

#include "Components/NameComponent.h"
#include "Components/CollisionComponents/CollidableComponent.h"
#include "Components/CollisionComponents/BoxColliderComponent.h"
#include "Components/CollisionComponents/SphereColliderComponent.h"
#include "Components/SkyboxComponent.h"
#include "OpenGL/OpenGLMeshBuffer.h"

#include <Texture/TextureManager.h>
#include <iostream>

#include "Components/AIScriptComponent.h"
#include "Components/EnemyComponent.h"

EntityFactory::EntityFactory(EnttFacade* enttFacade)
    : m_entityFacade(enttFacade)
{
}

void EntityFactory::createEntitiesFromModel(const std::string& filepath, const std::string& name) {
    // Get the model loader (singleton or instance as needed)
    ModelLoader& loader = ModelLoader::getInstance();

    // Load model data from file.
    LoadedModel modelData = loader.loadModel(filepath);
    AssimpImporter importer;

    // For every mesh in the model data, create an entity and attach components.
    for (const auto& rawMesh : modelData.meshes) {
        // Create an entity.
        auto entity = m_entityFacade->createEntity();

        //Create and set up the name component.
        NameComponent name_component = { name };
        m_entityFacade->addComponent<NameComponent>(entity, name_component);

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

void EntityFactory::createCollidableBoxEntitiesFromModel(const std::string& filepath, const std::string& name) {
    ModelLoader& loader = ModelLoader::getInstance();

    // Load the raw model data
    LoadedModel modelData = loader.loadModel(filepath);

    // Process raw meshes into MeshComponents
    AssimpImporter importer;
    for (const auto& rawMesh : modelData.meshes)
    {
        entt::entity entity = m_entityFacade->createEntity();

        NameComponent name_component = { name };
        m_entityFacade->addComponent<NameComponent>(entity, name_component);

        RenderableComponent meshComponent(rawMesh);
        importer.setupMesh(meshComponent);
        m_entityFacade->addComponent<RenderableComponent>(entity, meshComponent);

        TransformComponent transformComponent;
        transformComponent.setFromModelMatrix(rawMesh.transform);
        m_entityFacade->addComponent<TransformComponent>(entity, transformComponent);

        // for now im hard-coding the lighting shader into this, but it needs a way of being dynamically set
        MaterialComponent materialComponent(rawMesh.material, "lightingShader");
        m_entityFacade->addComponent<MaterialComponent>(entity, materialComponent);

        BoxColliderComponent collider = generateBoxCollider(extractPositions(meshComponent.vertices));
        m_entityFacade->addComponent<BoxColliderComponent>(entity, collider);
    }
}

void EntityFactory::createCollidableCapsuleEntitiesFromModel(const std::string& filepath, const std::string& name) {
    ModelLoader& loader = ModelLoader::getInstance();

    // Load the raw model data
    LoadedModel modelData = loader.loadModel(filepath);

    // Process raw meshes into MeshComponents
    AssimpImporter importer;
    for (const auto& rawMesh : modelData.meshes)
    {
        entt::entity entity = m_entityFacade->createEntity();

        NameComponent name_component = { name };
        m_entityFacade->addComponent<NameComponent>(entity, name_component);

        RenderableComponent meshComponent(rawMesh);
        importer.setupMesh(meshComponent);
        m_entityFacade->addComponent<RenderableComponent>(entity, meshComponent);

        TransformComponent transformComponent;
        transformComponent.setFromModelMatrix(rawMesh.transform);
        m_entityFacade->addComponent<TransformComponent>(entity, transformComponent);

        // for now im hard-coding the lighting shader into this, but it needs a way of being dynamically set
        MaterialComponent materialComponent(rawMesh.material, "lightingShader");
        m_entityFacade->addComponent<MaterialComponent>(entity, materialComponent);

        CapsuleColliderComponent collider = generateCapsuleCollider(extractPositions(meshComponent.vertices));
        m_entityFacade->addComponent<CapsuleColliderComponent>(entity, collider);
    }
}

void EntityFactory::createCollidableSphereEntitiesFromModel(const std::string& filepath, const std::string& name) {
    ModelLoader& loader = ModelLoader::getInstance();

  // Load the raw model data
  LoadedModel modelData = loader.loadModel(filepath);

  // Process raw meshes into MeshComponents
  AssimpImporter importer;
  for (const auto &rawMesh : modelData.meshes) {
    entt::entity entity = m_entityFacade->createEntity();

    NameComponent name_component = { name };
        m_entityFacade->addComponent<NameComponent>(entity, name_component);RenderableComponent meshComponent(rawMesh);
    importer.setupMesh(meshComponent);
    m_entityFacade->addComponent<RenderableComponent>(entity, meshComponent);

    TransformComponent transformComponent;
    transformComponent.setFromModelMatrix(rawMesh.transform);
    m_entityFacade->addComponent<TransformComponent>(entity,
                                                     transformComponent);

    // for now im hard-coding the lighting shader into this, but it needs a way
    // of being dynamically set
    MaterialComponent materialComponent(rawMesh.material, "lightingShader");
    m_entityFacade->addComponent<MaterialComponent>(entity, materialComponent);

    SphereColliderComponent collider =
        generateSphereCollider(extractPositions(meshComponent.vertices));
    m_entityFacade->addComponent<SphereColliderComponent>(entity, collider);
  }
}



void EntityFactory::createSkyBox(const std::array<std::string, 6>& faces) {
    // 1. Load the cubemap texture from the provided faces
    uint32_t cubeMapID = TextureManager::getInstance().loadCubeMapFromFiles(faces, "skybox");
    std::cout << "Cubemap ID: " << cubeMapID << std::endl;

    // 2. Prepare RawMeshData for a cube.
    // We'll use 36 vertices (two triangles per face, 6 faces) with positions only.
    RawMeshData skyboxMesh;
    std::vector<Vertex> vertices = {
        // Back face (z = -1)
        { glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },

        { glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },

        // Front face (z = 1)
        { glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },

        { glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },

        // Left face (x = -1)
        { glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },

        { glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },

        // Right face (x = 1)
        { glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },

        { glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },

        // Bottom face (y = -1)
        { glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },

        { glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },

        // Top face (y = 1)
        { glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },

        { glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
        { glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) }
    };

    // Generate sequential indices.
    std::vector<unsigned int> indices(vertices.size());
    for (unsigned int i = 0; i < vertices.size(); ++i)
        indices[i] = i;

    // Set up our mesh data.
    skyboxMesh.vertices = vertices;
    skyboxMesh.indices = indices;
    // Scale the cube to a very large size so it encompasses the scene.
    skyboxMesh.transform = glm::scale(glm::mat4(1.0f), glm::vec3(100.0f));
    auto entity = m_entityFacade->createEntity();

    AssimpImporter importer;


    RenderableComponent renderable(skyboxMesh);
    importer.setupMesh(renderable);
    m_entityFacade->addComponent<RenderableComponent>(entity, renderable);
    std::cout << "Skybox Renderable component " << std::endl;

    SkyboxComponent skyboxComponent;
    skyboxComponent.cubemapID = cubeMapID;
    skyboxComponent.shaderID = "skyboxShader";
    m_entityFacade->addComponent<SkyboxComponent>(entity, skyboxComponent);
    std::cout << "Skybox skybox component " << std::endl;

    // Transform component.
    TransformComponent transform;
    transform.setFromModelMatrix(skyboxMesh.transform);
    m_entityFacade->addComponent<TransformComponent>(entity, transform);
    std::cout << "Skybox transform component " << std::endl;

}




// Helper function to extract vertices from meshData
std::vector<glm::vec3> EntityFactory::extractPositions(const std::vector<Vertex>& meshVertices) {
    std::vector<glm::vec3> positions;
    positions.reserve(meshVertices.size());
    for (const auto& v : meshVertices)
        positions.push_back(v.position);
    return positions;
}


void EntityFactory::createPlayerEntitiesFromModel(const std::string& filepath) {
    ModelLoader& loader = ModelLoader::getInstance();
    LoadedModel modelData = loader.loadModel(filepath);
    AssimpImporter importer;

    for (const auto& rawMesh : modelData.meshes) {
        auto entity = m_entityFacade->createEntity();

        NameComponent name_component = { "player" };
        m_entityFacade->addComponent<NameComponent>(entity, name_component);

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

        BoxColliderComponent collider = generateBoxCollider(extractPositions(meshComponent.vertices));
        m_entityFacade->addComponent<BoxColliderComponent>(entity, collider);
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

void EntityFactory::addWaterEntity(const Water& water) {
    RenderableComponent waterComponent(water.getMeshData());
    waterComponent.meshBuffer = std::make_shared<OpenGLMeshBuffer>(
        waterComponent.indices.size(),
        waterComponent.vertices.size(),
        waterComponent.indices.data(),
        waterComponent.vertices.data()
    );

    auto entity = m_entityFacade->createEntity();
    m_entityFacade->addComponent<RenderableComponent>(entity, waterComponent);

    TransformComponent transformComponent;
    transformComponent.setFromModelMatrix(water.getMeshData().transform);
    m_entityFacade->addComponent<TransformComponent>(entity, transformComponent);

    MaterialComponent materialComponent(water.getMeshData().material, "waterShader");
    m_entityFacade->addComponent<MaterialComponent>(entity, materialComponent);

    NameComponent nameComponent = {"water"};
    m_entityFacade->addComponent<NameComponent>(entity, nameComponent);
}

void EntityFactory::createEnemyEntitiesFromModel(const std::string& filepath, const std::string& name) {
    ModelLoader& loader = ModelLoader::getInstance();
    LoadedModel modelData = loader.loadModel(filepath);
    AssimpImporter importer;

    for (const auto& rawMesh : modelData.meshes) {
        auto entity = m_entityFacade->createEntity();

        NameComponent name_component = { name };
        m_entityFacade->addComponent<NameComponent>(entity, name_component);

        RenderableComponent meshComponent(rawMesh);
        importer.setupMesh(meshComponent);
        m_entityFacade->addComponent<RenderableComponent>(entity, meshComponent);

        TransformComponent transformComponent;
        transformComponent.setFromModelMatrix(rawMesh.transform);
        m_entityFacade->addComponent<TransformComponent>(entity, transformComponent);

        MaterialComponent materialComponent(rawMesh.material, "lightingShader");
        m_entityFacade->addComponent<MaterialComponent>(entity, materialComponent);

        EnemyComponent enemyComponent;
        m_entityFacade->addComponent<EnemyComponent>(entity, enemyComponent);

        AIScriptComponent aiScriptComponent;
        m_entityFacade->addComponent<AIScriptComponent>(entity, aiScriptComponent);

        BoxColliderComponent collider = generateBoxCollider(extractPositions(meshComponent.vertices));
        m_entityFacade->addComponent<BoxColliderComponent>(entity, collider);
    }
}
