//
// Created by Shaun on 2/10/2024.
//

#include "PhysicsWorld.h"


#include <Camera.h>
#include <Collision.h>
#include <iostream>
namespace MinPhysics
{
    PhysicsWorld& PhysicsWorld::getInstance()
    {
        static PhysicsWorld instance;
        return instance;
    }

    PhysicsWorld::PhysicsWorld() : octreeRoot(nullptr)
    {
    }

    void PhysicsWorld::initialize(const std::vector<std::pair<std::string, std::unique_ptr<BaseActor>>>& objects)
    {
        glm::vec3 worldMin(std::numeric_limits<float>::max());
        glm::vec3 worldMax(std::numeric_limits<float>::lowest());

        for (const auto& [name, object] : objects) {
            glm::vec3 objMin, objMax;
            object->computeBoundingBox(objMin, objMax);

            worldMin = glm::min(worldMin, objMin);
            worldMax = glm::max(worldMax, objMax);
        }

        BoxCollider worldBounds(worldMin, worldMax);

        // Initialize the octree root node
        octreeRoot = new OctreeNode(worldBounds);

        // extract object portion of pair and insert object into the octree
        for (const auto& [name, object] : objects)
        {
            octreeRoot->insert(object.get());
        }

        // DEPRECATED
        // DebugRenderer::getInstance().initBoundingBoxRenderer();
    }

    PhysicsWorld::~PhysicsWorld()
    {
        delete octreeRoot;
        octreeRoot = nullptr;
    }

    void PhysicsWorld::update(Camera& camera, float deltaTime, std::map<int, bool>& flags)
    {

        // Reset camera acceleration
        camera.setAcceleration(glm::vec3(0.0f));

        // Apply gravity if the camera is not grounded, and if the debug free camera is off
        if (!camera.isGrounded())
        {
            camera.setAcceleration(camera.getAcceleration() + GRAVITY);
            // std::cout << "not grounded" << std::endl;
        }

        // Update velocity
        glm::vec3 velocity = camera.getVelocity();
        velocity += camera.getAcceleration() * deltaTime;

        // Limit maximum fall speed
        if (velocity.y < MAX_FALL_SPEED)
        {
            velocity.y = MAX_FALL_SPEED;
        }

        camera.setVelocity(velocity);

        // Update position
        glm::vec3 position = camera.getPosition();
        position += velocity * deltaTime;
        camera.setPosition(position);

        // Update the camera's collider
        camera.getCollider().center = position;


        Collision::checkCollisions(camera, getOctreeRoot());

        // After collision resolution, update grounded state
        if (camera.isGrounded())
        {
            // Apply friction to horizontal components
            velocity.x *= GROUND_FRICTION;
            velocity.z *= GROUND_FRICTION;

            // Zero out small velocities to prevent sliding due to floating-point errors
            const float VELOCITY_EPSILON = 1e-4f;
            if (std::abs(velocity.x) < 0.5)
                velocity.x = 0.0f;
            if (std::abs(velocity.z) < 0.5)
                velocity.z = 0.0f;

            // std::cout << "x velocity: " << velocity.x << " z velocity: " << velocity.z << std::endl;

            // Reset vertical velocity to zero to prevent downward movement
            velocity.y = 0.0f;

            // Update the camera's velocity
            camera.setVelocity(velocity);
        }
    }
}
