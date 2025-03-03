/**
* @file SphereCollider.h
 * @author Shaun Matthews
 * @date 21/09/2024
 * @brief Declaration of the SphereCollider class.
 * Creates and manages an sphere used for spatial collision detection.
 *
 *
 */

#ifndef SPHERECOLLIDER_H
#define SPHERECOLLIDER_H
#include <glm/glm.hpp>

namespace MinPhysics
{
    /// Forward declaration of box
    class BoxCollider;

    /**
     * @class SphereCollider
     * @brief A class representing a spherical collider used for spatial collision detection.
     */
    class SphereCollider {
    public:
        /// The center point of the sphere.
        glm::vec3 center;
        /// The radius of the sphere.
        float radius;

        /**
         * @brief Constructs a SphereCollider with the specified center and radius.
         *
         * @param center The center point of the sphere.
         * @param radius The radius of the sphere.
         */
        SphereCollider(const glm::vec3& center, float radius);

        /**
         * @brief Checks if this SphereCollider intersects with another SphereCollider.
         *
         * @param other The other SphereCollider to check for intersection.
         * @return True if the two SphereColliders intersect, false otherwise.
         */
        bool intersects(const SphereCollider& other) const;

        /**
         * @brief Checks if this SphereCollider intersects with a BoxCollider.
         *
         * @param box The BoxCollider to check for intersection.
         * @return True if the SphereCollider and BoxCollider intersect, false otherwise.
         */
        bool intersects(const BoxCollider& box) const;

        void setRadius(float newRadius) { radius = newRadius; }
    };
}
#endif // SPHERECOLLIDER_H
