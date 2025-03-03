/**
* @file PhysicsWorld.h
 * @author Shaun Matthews
 * @date 2/10/2024
 * @brief Declaration of the PhysicsWorld class.
 * Manages the physics and collision detection for the game engine.
 *
 */

#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H

#include "Octree.h"

#include <memory>
#include <vector>
#include <string>
#include <map>
class Camera;

namespace MinPhysics
{
    class OctreeNode;
    /**
     * @class PhysicsWorld
     * @brief Singleton class that manages the physics environment, including objects and spatial partitioning.
     *
     * The PhysicsWorld class maintains an octree structure to handle object collisions
     * and manages dynamic updates of objects in the physics simulation.
     */
    class PhysicsWorld
    {
    private:
     /// Root node of the octree used for spatial partitioning.
     OctreeNode* octreeRoot;


     /// Gravity constant
     static constexpr glm::vec3 GRAVITY = glm::vec3(0.0f, -9.81f, 0.0f);
     /// Maximum fall speed
     static constexpr float MAX_FALL_SPEED = -50.0f;
     static constexpr float GROUND_FRICTION = 0.8f; // Adjust between 0.0f (no friction) and 1.0f (full friction)


 public:

     /**
         * @brief Gets the singleton instance of the PhysicsWorld.
         *
         * @return A reference to the singleton PhysicsWorld instance.
         */
     static PhysicsWorld& getInstance();

     /**
         * @brief Initializes the physics world with a list of objects.
         *
         * This method sets up the octree and adds all objects to it for collision detection and spatial queries.
         *
         * @param objects A vector of unique pointers to objects to be managed by the physics world.
         */
     void PhysicsWorld::initialize(const std::vector<std::pair<std::string, std::unique_ptr<BaseActor>>>& objects);

     /**
         * @brief Gets the root node of the octree.
         *
         * @return A pointer to the root node of the octree.
         */
     OctreeNode* getOctreeRoot() const { return octreeRoot; }

     /**
         * @brief Updates the physics world.
         *
         * This method can be used to update dynamic objects in the physics simulation.
         */
     void update(Camera& camera, float deltaTime, std::map<int, bool>& flags);

     /**
         * @brief Destructor for PhysicsWorld, cleans up resources.
         */
     ~PhysicsWorld();

    private:
     /**
         * @brief Private constructor for the PhysicsWorld singleton.
         */
     PhysicsWorld();
     /**
         * @brief Deleted copy constructor to prevent copying of the singleton.
         */
     PhysicsWorld(const PhysicsWorld&) = delete;

     /**
         * @brief Deleted assignment operator to prevent assignment of the singleton.
         *
         * @return A reference to the current PhysicsWorld instance.
         */
     PhysicsWorld& operator=(const PhysicsWorld&) = delete;
    };

}

#endif //PHYSICSWORLD_H
