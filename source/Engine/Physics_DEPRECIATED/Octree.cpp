//
// Created by Shaun on 30/09/2024.
//

#include "Octree.h"

namespace MinPhysics
{
    // these magical values seem to work quite well
    const int MAX_OBJECTS_PER_NODE = 10;
    const int MAX_OCTREE_DEPTH = 10;

    // AABB OctreeNode::worldBounds = AABB(glm::vec3(0.0f), glm::vec3(1000.0f));

    OctreeNode::OctreeNode(const BoxCollider& bounds) : bounds(bounds) {
        for (int i = 0; i < 8; ++i) {
            children[i] = nullptr;
        }
    }

    OctreeNode::~OctreeNode() {
        for (int i = 0; i < 8; ++i) {
            if (children[i] != nullptr) {
                delete children[i];
                children[i] = nullptr;
            }
        }
    }

void OctreeNode::insert(BaseActor* object, int depth)
    {
        // Check if the object is collidable
        if (!object->isCollidable())
        {
            // Do not insert non-collidable objects into the octree
            return;
        }

        // Compute the object's bounding box
        glm::vec3 objMin, objMax;
        object->computeBoundingBox(objMin, objMax);
        BoxCollider objectBounds(objMin, objMax);

        // If the object's bounding box doesn't intersect this node's bounds, do nothing
        if (!bounds.intersects(objectBounds))
            return;

        // If we have children, try inserting into children
        if (children[0] != nullptr)
        {
            for (int i = 0; i < 8; ++i)
            {
                children[i]->insert(object, depth + 1);
            }
            return;
        }

        // Add object to this node
        objects.push_back(object);

        // Subdivide if necessary
        if (objects.size() > MAX_OBJECTS_PER_NODE && depth < MAX_OCTREE_DEPTH)
        {
            subdivide();
            // Re-insert objects into children
            for (auto it = objects.begin(); it != objects.end();)
            {
                bool insertedIntoChild = false;
                for (int i = 0; i < 8; ++i)
                {
                    glm::vec3 childMin, childMax;
                    (*it)->computeBoundingBox(childMin, childMax);
                    BoxCollider childObjectBounds(childMin, childMax);

                    if (children[i]->bounds.intersects(childObjectBounds))
                    {
                        children[i]->insert(*it, depth + 1);
                        insertedIntoChild = true;
                    }
                }
                if (insertedIntoChild)
                {
                    it = objects.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
    }




    void OctreeNode::subdivide() {
        glm::vec3 center = (bounds.min + bounds.max) * 0.5f;
        // Create 8 child nodes with appropriate bounds
        for (int i = 0; i < 8; ++i) {
            BoxCollider childBounds = calculateChildBounds(bounds, center, i);
            children[i] = new OctreeNode(childBounds);
        }
    }

    void OctreeNode::retrieve(const SphereCollider& collider, std::vector<BaseActor*>& potentialColliders)
    {
        if (!bounds.intersects(collider))
            return;

        // Add only collidable objects in this node
        for (BaseActor* object : objects)
        {
            if (object->isCollidable())
            {
                potentialColliders.push_back(object);
            }
        }

        // Recurse into children
        if (children[0] != nullptr)
        {
            for (int i = 0; i < 8; ++i)
            {
                if (children[i])
                {
                    children[i]->retrieve(collider, potentialColliders);
                }
            }
        }
    }

    void OctreeNode::retrieve(const BoxCollider& collider, std::vector<BaseActor*>& potentialColliders) {
        if (!bounds.intersects(collider)) return;

        potentialColliders.insert(potentialColliders.end(), objects.begin(), objects.end());

        if (children[0] != nullptr) {
            for (int i = 0; i < 8; ++i) {
                children[i]->retrieve(collider, potentialColliders);
            }
        }
    }

    void OctreeNode::updateObject(BaseActor* object) {
        // Remove the object from the octree
        remove(object);
        // Re-insert the object into the octree with its new position
        insert(object);
    }

    bool OctreeNode::contains(const BoxCollider& box) const {
        // Check if 'box' is fully inside 'this->bounds'
        return (box.min.x >= bounds.min.x && box.max.x <= bounds.max.x) &&
               (box.min.y >= bounds.min.y && box.max.y <= bounds.max.y) &&
               (box.min.z >= bounds.min.z && box.max.z <= bounds.max.z);
    }

    void OctreeNode::remove(BaseActor* object) {
        // If object is not in this node and has children, try removing from children
        if (!objects.empty()) {
            auto it = std::find(objects.begin(), objects.end(), object);
            if (it != objects.end()) {
                objects.erase(it);
                return;
            }
        }
        // Recurse into children
        if (children[0] != nullptr) {
            for (int i = 0; i < 8; ++i) {
                children[i]->remove(object);
            }
        }
    }

    // Definition of calculateChildBounds
    BoxCollider calculateChildBounds(const BoxCollider& parentBounds, const glm::vec3& center, int index) {
        glm::vec3 min = parentBounds.min;
        glm::vec3 max = parentBounds.max;
        glm::vec3 newMin, newMax;

        // Determine which octant based on the index (0 to 7)
        newMin.x = (index & 1) ? center.x : min.x;
        newMax.x = (index & 1) ? max.x    : center.x;

        newMin.y = (index & 2) ? center.y : min.y;
        newMax.y = (index & 2) ? max.y    : center.y;

        newMin.z = (index & 4) ? center.z : min.z;
        newMax.z = (index & 4) ? max.z    : center.z;

        return BoxCollider(newMin, newMax);
    }
}