//
// Created by Shaun on 17/05/2025.
//

#ifndef ENTITYBUILDER_H
#define ENTITYBUILDER_H

#include <glm/fwd.hpp>

#include "Components/AIScriptComponent.h"
#include "Components/TransformComponent.h"
#include "Components/CollisionComponents/BoxColliderComponent.h"
#include "Components/CollisionComponents/CapsuleColliderComponent.h"
#include "Components/CollisionComponents/SphereColliderComponent.h"
#include "Importers/ModelLoader.h"
#include "ResourceManagement/EnttFacade.h"
#include "Terrain/Terrain.h"
#include "Water/Water.h"

/*
 * This is designed to be an internal core class for the creation of entities.
 * It is not intended to be client facing, it should only be accessed through an interface. 
 */
class EntityBuilder
{
public:
    explicit EntityBuilder(EnttFacade* entityFacade);
    
    // Start building a new entity (what kind of breakfast is this?)
    EntityBuilder& createEntity();
    
    // Add components (add your own toppings, bacon?)
    EntityBuilder& withName(const std::string& name);
    EntityBuilder& withTransform(const glm::mat4& transform = glm::mat4(1.0f));
    EntityBuilder& withPosition(const glm::vec3& position);
    EntityBuilder& withRotation(const glm::vec3& rotation);
    EntityBuilder& withScale(const glm::vec3& scale);
    
    // Model loading methods (what will it look like?)
    EntityBuilder& fromModel(const std::string& filepath);
    EntityBuilder& withMaterial(const std::string& shaderName);
    
    // Collision components (how will it interact with the world?)
    EntityBuilder& withBoxCollider();
    EntityBuilder& withSphereCollider();
    EntityBuilder& withCapsuleCollider();
    EntityBuilder& withAI(AIScriptComponent::State initialState = AIScriptComponent::patrol);
    
    // Special entity types (what kind of entity is this?)
    EntityBuilder& asPlayer(int playerID = 1);
    EntityBuilder& asNPC(const std::string& npcType);
    EntityBuilder& asEnemy();
   
    // Finalize and build the entity
    entt::entity build();
    
    // Utility method to create multiple entities from one model
    std::vector<entt::entity> buildMultiple();

    
private:
    EnttFacade* m_entityFacade;
    entt::entity m_currentEntity;
    
    // State for the current entity being built
    std::string m_name;
    TransformComponent m_transform;
    bool m_hasModel = false;
    std::string m_modelPath;
    std::string m_shaderName = "lightingShader";
    bool m_isPlayer = false;
    int m_playerID = 0;
    bool m_isNPC = false;
    std::string m_npcType;
    bool m_isEnemy = false;
    bool m_hasAI = false;
    AIScriptComponent::State m_initialAIState = AIScriptComponent::patrol;
    
    // BehaviorType m_behaviorType = BehaviorType::None;
    // std::vector<glm::vec3> m_waypoints;

    
    // TODO not sure if we really need these in here 
    enum class ColliderType {
        None,
        Box,
        Sphere,
        Capsule
    };
    enum class BehaviorType {
        None,
        Patrol,
        Guard,
        Follow,
        Flee
    };
    
    ColliderType m_colliderType = ColliderType::None;
    
    // Model data for processing
    LoadedModel m_modelData;
    bool m_modelLoaded = false;
    
    // Helper methods
    void loadModelData();
    std::vector<glm::vec3> extractPositions(const std::vector<Vertex>& meshVertices);
    BoxColliderComponent generateBoxCollider(const std::vector<glm::vec3>& positions);
    SphereColliderComponent generateSphereCollider(const std::vector<glm::vec3>& positions);
    CapsuleColliderComponent generateCapsuleCollider(const std::vector<glm::vec3>& positions);
    
    // Reset builder state after build
    void reset();
};

#endif //ENTITYBUILDER_H
