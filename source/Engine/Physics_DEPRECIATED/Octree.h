/**
* @file Octree.h
* @brief A quadrant-based tree data structure for handling spatial collision detection.
 *
 * This file defines the OctreeNode class, which is used to manage spatial partitioning
 * and efficiently handle object collision detection in 3D space.
 *
 * @author Shaun Matthews
 * @date Created: 29/09/2024
 * @date Modified: 28/01/25
 * @TODO Replace BaseActor with a more suitable class or "entity" since BaseActor is deprecated
 *      as I moved to using ECS. Currently BaseActor has been substituted with MeshComponent.
 */

#ifndef OCTREE_H
#define OCTREE_H

#include "BoxCollider.h"
#include <vector>

#include "BaseActor.h"
#include "Components/MeshComponent.h"

namespace MinPhysics
{
    /**
 * @class OctreeNode
 * @brief Represents a node in an octree spatial partitioning structure.
 *
 * The OctreeNode class is responsible for partitioning space into smaller regions (octants)
 * and managing objects within these regions for efficient spatial queries and collision detection.
 */
    class OctreeNode
    {
    public:
        BoxCollider bounds; /// Bounding box of the node

        // TODO BaseActor is deprecated, this needs to be replaced with something more suitable
        // std::vector<BaseActor*> objects; /// Objects contained in this node
        OctreeNode* children[8]; /// Child nodes
        /**
        * @brief Constructs an OctreeNode with a specified bounding box.
        *
        * @param bounds The bounding box defining the region of this node.
        */
        explicit OctreeNode(const BoxCollider& bounds);
        /**
         * @brief Deleted default constructor to prevent creation without bounds.
         */
        OctreeNode() = delete;
        /**
        * @brief Destructor for OctreeNode. Cleans up child nodes and objects.
        */
        ~OctreeNode();

        /**
        * @brief Inserts an object into the octree node.
        *
        * Subdivides the node if necessary and adds the object to the appropriate child or the current node.
        *
        * @param object Pointer to the object to be inserted.
        * @param depth The depth level of the node in the octree (default is 0).
        */
        void insert(MeshComponent* object, int depth = 0);
        /**
         * @brief Retrieves objects potentially colliding with a given BoxCollider.
         *
         * Searches the node and its children for objects that intersect with the specified BoxCollider.
         *
         * @param collider The bounding box used to search for potential collisions.
         * @param potentialColliders Vector to store pointers to potential colliding objects.
         */
        void retrieve(const BoxCollider& collider, std::vector<MeshComponent*>& potentialColliders);

        /**
        * @brief Retrieves objects potentially colliding with a given SphereCollider.
        *
        * Searches the node and its children for objects that intersect with the specified SphereCollider.
        *
        * @param collider The sphere used to search for potential collisions.
        * @param potentialColliders Vector to store pointers to potential colliding objects.
        */
        void retrieve(const SphereCollider& collider, std::vector<MeshComponent*>& potentialColliders);

        /**
         * @brief Updates the position of an object within the octree.
         *
         * This function repositions the object within the tree if it has moved out of the current node.
         *
         * @param object Pointer to the object to be updated.
         */
        void updateObject(MeshComponent* object);

        /**
        * @brief Removes an object from the octree node.
        *
        * Removes the object from the node and rebalances if necessary.
        *
        * @param object Pointer to the object to be removed.
        */
        void remove(MeshComponent* object);

        void collapse();

       private:
        /**
        * @brief Subdivides the node into 8 child nodes.
        */
        void subdivide();

        /**
        * @brief Checks if a bounding box is contained within the current node's bounds.
        *
        * @param box The bounding box to check.
        * @return True if the box is contained within this node, false otherwise.
        */
        bool contains(const BoxCollider& box) const;
    };

    /**
     * @brief Calculates the bounding box for a child node.
     *
     * @param parentBounds The bounding box of the parent node.
     * @param center The center point of the parent node.
     * @param index The index of the child node (0-7).
     * @return The bounding box for the specified child node.
     */
    BoxCollider calculateChildBounds(const BoxCollider& parentBounds, const glm::vec3& center, int index);
}
#endif //OCTREE_H
