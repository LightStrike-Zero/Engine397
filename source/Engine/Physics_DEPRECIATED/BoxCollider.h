/**
* @file BoxCollider.h
 * @author Shaun Matthews
 * @date 21/09/2024
 * @brief Declaration of the BoxCollider class.
 * Creates and manages an axis-aligned bounding box (AABB) used for spatial collision detection.
 *
 *
 */

#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include <glm/glm.hpp>
namespace MinPhysics
{
    /// Forward declaration of sphere
    class SphereCollider;

    /**
     * @class BoxCollider
     * @brief A class representing an axis-aligned bounding box (AABB) used for spatial collision detection.
     */
    class BoxCollider {
    public:
        /// The minimum point of the bounding box.
        glm::vec3 min;
        /// The maximum point of the bounding box.
        glm::vec3 max;

        /**
         * @brief Constructs a BoxCollider with the specified minimum and maximum points.
         *
         * @param min The minimum point of the box.
         * @param max The maximum point of the box.
         */
        BoxCollider(const glm::vec3& min, const glm::vec3& max);

        /**
         * @brief Checks if this BoxCollider intersects with another BoxCollider.
         *
         * @param other The other BoxCollider to check for intersection.
         * @return True if the two BoxColliders intersect, false otherwise.
         */
        bool intersects(const BoxCollider& other) const;

        /**
         * @brief Checks if this BoxCollider intersects with a SphereCollider.
         *
         * @param sphere The SphereCollider to check for intersection.
         * @return True if the BoxCollider and SphereCollider intersect, false otherwise.
         */
        bool intersects(const SphereCollider& sphere) const;
    };
}
#endif //BOXCOLLIDER_H
