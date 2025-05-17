#include "EntityBuilder.h"
#include "OpenGL/OpenGLMeshBuffer.h"
#include <Texture/TextureManager.h>
#include <iostream>
#include <glm/gtx/norm.inl>

#include "Components/MaterialComponent.h"
#include "Components/PlayerControllerComponent.h"
#include "Components/SkyboxComponent.h"

EntityBuilder::EntityBuilder(EnttFacade* entityFacade)
    : m_entityFacade(entityFacade), m_currentEntity(entt::null) {
}

EntityBuilder& EntityBuilder::createEntity() {
    // Reset any previous state
    reset();
    // Create a new entity
    m_currentEntity = m_entityFacade->createEntity();
    return *this;
}

EntityBuilder& EntityBuilder::withName(const std::string& name) {
    m_name = name;
    return *this;
}

EntityBuilder& EntityBuilder::withTransform(const glm::mat4& transform) {
    m_transform.setFromModelMatrix(transform);
    return *this;
}

EntityBuilder& EntityBuilder::withPosition(const glm::vec3& position) {
    m_transform.position = position;
    return *this;
}

EntityBuilder& EntityBuilder::withRotation(const glm::vec3& rotation) {
    m_transform.rotation = rotation;
    return *this;
}

EntityBuilder& EntityBuilder::withScale(const glm::vec3& scale) {
    m_transform.scale = scale;
    return *this;
}

EntityBuilder& EntityBuilder::fromModel(const std::string& filepath) {
    m_hasModel = true;
    m_modelPath = filepath;
    return *this;
}

EntityBuilder& EntityBuilder::withMaterial(const std::string& shaderName) {
    m_shaderName = shaderName;
    return *this;
}

EntityBuilder& EntityBuilder::withBoxCollider() {
    m_colliderType = ColliderType::Box;
    return *this;
}

EntityBuilder& EntityBuilder::withSphereCollider() {
    m_colliderType = ColliderType::Sphere;
    return *this;
}

EntityBuilder& EntityBuilder::withCapsuleCollider() {
    m_colliderType = ColliderType::Capsule;
    return *this;
}

EntityBuilder& EntityBuilder::asPlayer(int playerID) {
    m_isPlayer = true;
    m_playerID = playerID;
    return *this;
}

EntityBuilder& EntityBuilder::asNPC(const std::string& npcType) {
    m_isNPC = true;
    m_npcType = npcType;
    return *this;
}

// EntityBuilder& EntityBuilder::withBehavior(BehaviorType behaviorType, const std::vector<glm::vec3>& waypoints) {
//     m_behaviorType = behaviorType;
//     m_waypoints = waypoints;
//     return *this;
// }

// EntityBuilder& EntityBuilder::asSkybox(const std::array<std::string, 6>& faces) {
//     m_isSkybox = true;
//     m_skyboxFaces = faces;
//     return *this;
// }
//
// EntityBuilder& EntityBuilder::asTerrain(const Terrain& terrain) {
//     m_isTerrain = true;
//     m_terrain = &terrain;
//     return *this;
// }
//
// EntityBuilder& EntityBuilder::asWater(const Water& water) {
//     m_isWater = true;
//     m_water = &water;
//     return *this;
// }

entt::entity EntityBuilder::build() {
    if (m_currentEntity == entt::null) {
        std::cerr << "Error: Trying to build without creating an entity first." << std::endl;
        return entt::null;
    }

    // Add name component if provided
    if (!m_name.empty()) {
        NameComponent nameComponent = { m_name };
        m_entityFacade->addComponent<NameComponent>(m_currentEntity, nameComponent);
    }

    // Always add transform component
    m_entityFacade->addComponent<TransformComponent>(m_currentEntity, m_transform);

    // Add NPC-specific components
    if (m_isNPC) {
        // We need to create these components first
        // For now, just leaving a comment as placeholder
        // TODO: Implement NPCComponent and BehaviorComponent classes
        
        // NPCComponent npcComponent { m_npcType };
        // m_entityFacade->addComponent<NPCComponent>(m_currentEntity, npcComponent);

        // // Add behavior component if specified
        // if (m_behaviorType != BehaviorType::None) {
        //     BehaviorComponent behaviorComponent { m_behaviorType, m_waypoints };
        //     m_entityFacade->addComponent<BehaviorComponent>(m_currentEntity, behaviorComponent);
        // }
    }

    // Load model if specified
    if (m_hasModel) {
        loadModelData();

        // Process the first mesh (should be fine for single-mesh models)
        if (!m_modelData.meshes.empty()) {
            const auto& rawMesh = m_modelData.meshes[0];

            // Create renderable component
            RenderableComponent meshComponent(rawMesh);
            AssimpImporter importer;
            importer.setupMesh(meshComponent);
            m_entityFacade->addComponent<RenderableComponent>(m_currentEntity, meshComponent);

            // Set transform from model if not explicitly set
            if (m_transform.position == glm::vec3(0.0f) &&
                m_transform.rotation == glm::vec3(0.0f) &&
                m_transform.scale == glm::vec3(1.0f)) {
                TransformComponent transformComponent;
                transformComponent.setFromModelMatrix(rawMesh.transform);
                m_entityFacade->addComponent<TransformComponent>(m_currentEntity, transformComponent);
            }

            // Add material component
            MaterialComponent materialComponent(rawMesh.material, m_shaderName);
            m_entityFacade->addComponent<MaterialComponent>(m_currentEntity, materialComponent);

            // Add collision component if requested
            if (m_colliderType != ColliderType::None) {
                std::vector<glm::vec3> positions = extractPositions(meshComponent.vertices);
                
                switch (m_colliderType) {
                    case ColliderType::Box: {
                        BoxColliderComponent collider = generateBoxCollider(positions);
                        m_entityFacade->addComponent<BoxColliderComponent>(m_currentEntity, collider);
                        break;
                    }
                    case ColliderType::Sphere: {
                        SphereColliderComponent collider = generateSphereCollider(positions);
                        m_entityFacade->addComponent<SphereColliderComponent>(m_currentEntity, collider);
                        break;
                    }
                    case ColliderType::Capsule: {
                        CapsuleColliderComponent collider = generateCapsuleCollider(positions);
                        m_entityFacade->addComponent<CapsuleColliderComponent>(m_currentEntity, collider);
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }

    // Add player controller if requested
    if (m_isPlayer) {
        PlayerControllerComponent controllerComponent { m_playerID };
        m_entityFacade->addComponent<PlayerControllerComponent>(m_currentEntity, controllerComponent);
    }

    // Store the created entity ID
    entt::entity result = m_currentEntity;
    
    // Reset after build completes
    reset();
    
    return result;
}

std::vector<entt::entity> EntityBuilder::buildMultiple() {
    std::vector<entt::entity> entities;
    
    if (m_hasModel) {
        loadModelData();
        
        // Create an entity for each mesh in the model
        for (const auto& rawMesh : m_modelData.meshes) {
            // Create a new entity for this mesh
            entt::entity entity = m_entityFacade->createEntity();
            entities.push_back(entity);
            
            // Add name component if provided
            if (!m_name.empty()) {
                NameComponent nameComponent = { m_name };
                m_entityFacade->addComponent<NameComponent>(entity, nameComponent);
            }
            
            // Create renderable component
            RenderableComponent meshComponent(rawMesh);
            AssimpImporter importer;
            importer.setupMesh(meshComponent);
            m_entityFacade->addComponent<RenderableComponent>(entity, meshComponent);
            
            // Add transform component
            TransformComponent transformComponent;
            transformComponent.setFromModelMatrix(rawMesh.transform);
            // Override with custom transform values if specified
            if (m_transform.position != glm::vec3(0.0f)) {
                transformComponent.position = m_transform.position;
            }
            if (m_transform.rotation != glm::vec3(0.0f)) {
                transformComponent.rotation = m_transform.rotation;
            }
            if (m_transform.scale != glm::vec3(1.0f)) {
                transformComponent.scale = m_transform.scale;
            }
            m_entityFacade->addComponent<TransformComponent>(entity, transformComponent);
            
            // Add material component
            MaterialComponent materialComponent(rawMesh.material, m_shaderName);
            m_entityFacade->addComponent<MaterialComponent>(entity, materialComponent);
            
            // Add collision component if requested
            if (m_colliderType != ColliderType::None) {
                std::vector<glm::vec3> positions = extractPositions(meshComponent.vertices);
                
                switch (m_colliderType) {
                    case ColliderType::Box:
                        m_entityFacade->addComponent<BoxColliderComponent>(
                            entity, 
                            generateBoxCollider(positions)
                        );
                        break;
                    case ColliderType::Sphere:
                        m_entityFacade->addComponent<SphereColliderComponent>(
                            entity, 
                            generateSphereCollider(positions)
                        );
                        break;
                    case ColliderType::Capsule:
                        m_entityFacade->addComponent<CapsuleColliderComponent>(
                            entity, 
                            generateCapsuleCollider(positions)
                        );
                        break;
                    default:
                        break;
                }
            }
            
            // Add player controller if requested
            if (m_isPlayer) {
                PlayerControllerComponent controllerComponent { m_playerID };
                m_entityFacade->addComponent<PlayerControllerComponent>(entity, controllerComponent);
            }
        }
    }
    
    return entities;
}

// Private methods

void EntityBuilder::reset() {
    m_currentEntity = entt::null;
    m_name = "";
    m_transform = TransformComponent();
    m_hasModel = false;
    m_modelPath = "";
    m_shaderName = "lightingShader";
    m_isPlayer = false;
    m_playerID = 0;
    m_isNPC = false;
    m_npcType = "";
    // m_behaviorType = BehaviorType::None;
    // m_waypoints.clear();
    m_colliderType = ColliderType::None;
    m_modelLoaded = false;
}

void EntityBuilder::loadModelData() {
    if (!m_hasModel || m_modelPath.empty()) {
        std::cerr << "Error: Cannot load model data, no model path specified" << std::endl;
        return;
    }

    ModelLoader& loader = ModelLoader::getInstance();
    m_modelData = loader.loadModel(m_modelPath);
    m_modelLoaded = true;
}

// entt::entity EntityBuilder::createSkyboxEntity(const std::array<std::string, 6>& faces) {
//     // Load cubemap texture
//     uint32_t cubeMapID = TextureManager::getInstance().loadCubeMapFromFiles(m_skyboxFaces, "skybox");
//     
//     // Create skybox mesh (cube)
//     RawMeshData skyboxMesh;
//     std::vector<Vertex> vertices = {
//         // Back face (z = -1)
//         { glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//
//         { glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//
//         // Front face (z = 1)
//         { glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//
//         { glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//
//         // Left face (x = -1)
//         { glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//
//         { glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//
//         // Right face (x = 1)
//         { glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//
//         { glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//
//         // Bottom face (y = -1)
//         { glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//
//         { glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//
//         // Top face (y = 1)
//         { glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//
//         { glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) },
//         { glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.f,0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f) }
//     };
//     // Generate sequential indices
//     std::vector<unsigned int> indices(vertices.size());
//     for (unsigned int i = 0; i < vertices.size(); ++i)
//         indices[i] = i;
//
//     skyboxMesh.vertices = vertices;
//     skyboxMesh.indices = indices;
//     // Scale the cube to a very large size
//     skyboxMesh.transform = glm::scale(glm::mat4(1.0f), glm::vec3(100.0f));
//
//     // Use the current entity or create a new one if needed
//     if (m_currentEntity == entt::null) {
//         m_currentEntity = m_entityFacade->createEntity();
//     }
//
//     // Create renderable component
//     RenderableComponent renderable(skyboxMesh);
//     AssimpImporter importer;
//     importer.setupMesh(renderable);
//     m_entityFacade->addComponent<RenderableComponent>(m_currentEntity, renderable);
//
//     // Create skybox component
//     SkyboxComponent skyboxComponent;
//     skyboxComponent.cubemapID = cubeMapID;
//     skyboxComponent.shaderID = "skyboxShader";
//     m_entityFacade->addComponent<SkyboxComponent>(m_currentEntity, skyboxComponent);
//
//     // Create transform component
//     TransformComponent transform;
//     transform.setFromModelMatrix(skyboxMesh.transform);
//     m_entityFacade->addComponent<TransformComponent>(m_currentEntity, transform);
//
//     return m_currentEntity;
// }
//
// entt::entity EntityBuilder::createTerrainEntity(const Terrain& terrain) {
//     // Create renderable component from terrain mesh data
//     RenderableComponent terrainComponent(terrain.getMeshData());
//     terrainComponent.meshBuffer = std::make_shared<OpenGLMeshBuffer>(
//         terrainComponent.indices.size(),
//         terrainComponent.vertices.size(),
//         terrainComponent.indices.data(),
//         terrainComponent.vertices.data()
//     );
//
//     // Use the current entity or create a new one if needed
//     if (m_currentEntity == entt::null) {
//         m_currentEntity = m_entityFacade->createEntity();
//     }
//     
//     m_entityFacade->addComponent<RenderableComponent>(m_currentEntity, terrainComponent);
//
//     // Add transform component
//     TransformComponent transformComponent;
//     transformComponent.setFromModelMatrix(terrain.getMeshData().transform);
//     m_entityFacade->addComponent<TransformComponent>(m_currentEntity, transformComponent);
//
//     // Add material component
//     MaterialComponent materialComponent(terrain.getMeshData().material, "lightingShader");
//     m_entityFacade->addComponent<MaterialComponent>(m_currentEntity, materialComponent);
//
//     // Add name component if specified
//     if (!m_name.empty()) {
//         NameComponent nameComponent = { m_name };
//         m_entityFacade->addComponent<NameComponent>(m_currentEntity, nameComponent);
//     }
//
//     return m_currentEntity;
// }
//
// entt::entity EntityBuilder::createWaterEntity(const Water& water) {
//     // Create renderable component from water mesh data
//     RenderableComponent waterComponent(water.getMeshData());
//     waterComponent.meshBuffer = std::make_shared<OpenGLMeshBuffer>(
//         waterComponent.indices.size(),
//         waterComponent.vertices.size(),
//         waterComponent.indices.data(),
//         waterComponent.vertices.data()
//     );
//
//     // Use the current entity or create a new one if needed
//     if (m_currentEntity == entt::null) {
//         m_currentEntity = m_entityFacade->createEntity();
//     }
//     
//     m_entityFacade->addComponent<RenderableComponent>(m_currentEntity, waterComponent);
//
//     // Add transform component
//     TransformComponent transformComponent;
//     transformComponent.setFromModelMatrix(water.getMeshData().transform);
//     m_entityFacade->addComponent<TransformComponent>(m_currentEntity, transformComponent);
//
//     // Add material component
//     MaterialComponent materialComponent(water.getMeshData().material, "waterShader");
//     m_entityFacade->addComponent<MaterialComponent>(m_currentEntity, materialComponent);
//
//     // Add name component
//     NameComponent nameComponent = {m_name.empty() ? "water" : m_name};
//     m_entityFacade->addComponent<NameComponent>(m_currentEntity, nameComponent);
//
//     return m_currentEntity;
// }
std::vector<glm::vec3> EntityBuilder::extractPositions(const std::vector<Vertex>& meshVertices) {
    std::vector<glm::vec3> positions;
    positions.reserve(meshVertices.size());
    for (const auto& v : meshVertices) {
        positions.push_back(v.position);
    }
    return positions;
}

BoxColliderComponent EntityBuilder::generateBoxCollider(const std::vector<glm::vec3>& positions) {
    glm::vec3 min = positions[0];
    glm::vec3 max = positions[0];
    
    for (const auto& v : positions) {
        min = glm::min(min, v);
        max = glm::max(max, v);
    }
    
    glm::vec3 size = (max - min) * 0.5f; // Half extents
    glm::vec3 offset = (min + max) * 0.5f;
    
    return BoxColliderComponent{size, offset};
}

SphereColliderComponent EntityBuilder::generateSphereCollider(const std::vector<glm::vec3>& positions) {
    glm::vec3 center(0.0f);
    for (const auto& v : positions) center += v;
    center /= positions.size(); // average position
    
    float maxRadiusSq = 0.0f;
    for (const auto& v : positions)
        maxRadiusSq = std::max(maxRadiusSq, glm::distance2(v, center));
    
    float radius = std::sqrt(maxRadiusSq);
    return SphereColliderComponent{radius, center};
}

CapsuleColliderComponent EntityBuilder::generateCapsuleCollider(const std::vector<glm::vec3>& positions) {
    glm::vec3 min = positions[0];
    glm::vec3 max = positions[0];
    
    for (const auto& v : positions) {
        min = glm::min(min, v);
        max = glm::max(max, v);
    }
    
    float height = max.y - min.y;
    float radius = 0.0f;
    
    // Compute max radius from vertical axis (XZ)
    glm::vec3 centerXZ = glm::vec3((min.x + max.x) * 0.5f, 0.0f, (min.z + max.z) * 0.5f);
    for (const auto& v : positions) {
        glm::vec3 posXZ = glm::vec3(v.x, 0.0f, v.z);
        float dist = glm::length(posXZ - centerXZ);
        radius = std::max(radius, dist);
    }
    
    glm::vec3 offset = (min + max) * 0.5f;
    
    return CapsuleColliderComponent{radius, height, offset};
}