/**
* @file Collision.h
 * @author Shaun Matthews
 * @date 21/09/2024
 * @brief Declaration of the Physics class responsible for collision detection and resolution.
 */

#ifndef COLLISION_H
#define COLLISION_H

// =============================
// Standard Library Includes
// =============================
#include <vector>

// =============================
// Third-Party Library Includes
// =============================
#include <glm/glm.hpp>

// =============================
// Project-Specific Includes
// =============================
#include "BaseActor.h"
#include "Camera.h"

#include <Mesh.h>

class StaticMesh;
namespace MinPhysics
{
 class OctreeNode;
 /**
  * @struct CollisionInfo
  * @brief Contains information about a collision event.
  */
 struct CollisionInfo
 {
  glm::vec3 normal; // The normal vector of the surface collided with
  float penetrationDepth; // How deep the sphere is penetrating the surface
  BaseActor* collidedObject; // Pointer to the object collided with
  glm::vec3 v0, v1, v2; // Vertices of the triangle collided with
  glm::vec3 closestPoint; // Closest point on the triangle to the sphere center
 };

 /**
  * @class Collision
  * @brief Provides static methods for collision detection and resolution within the physics engine.
  */
 class Collision
 {
 public:
  /**
   * @brief Checks for collisions between the sphere and all objects in the scene.
   *
   * @param cam
   * @param octreeRoot

   * @return True if a collision is detected, false otherwise.
   */
  // static bool checkCollisions(Camera& cam, const std::vector<std::unique_ptr<Object>>& objects); this is the old version
  static bool checkCollisions(Camera& cam, OctreeNode* octreeRoot);

  /**
  * @brief Resolves a collision by adjusting the camera's position.
  *
  * This function moves the camera along the collision normal by the penetration depth
  * to resolve the collision.
  *
  * @param cam Reference to the Camera object.
  * @param normal The normal vector of the collision surface.
  * @param penetrationDepth The depth by which the camera has penetrated the collision surface.
  */
  static void resolveCollision(Camera& cam, const glm::vec3& normal, float penetrationDepth);

  /**
   * @brief Counter for the number of collision tests performed.
   *
   * This static member keeps track of how many collision tests have been executed.
   */
  static int collisionTestCounter;

 private:
  /**
   * @brief Checks for collision between a sphere and a triangle defined by three vertices.
   *
   * This function calculates the closest point on the triangle to the sphere's center
   * and checks if the distance between them is less than or equal to the sphere's radius.
   *
   * @param sphere The sphere collider.
   * @param v0 First vertex of the triangle.
   * @param v1 Second vertex of the triangle.
   * @param v2 Third vertex of the triangle.
   * @return True if the sphere intersects the triangle, false otherwise.
   */
  static bool sphereIntersectsTriangle(const SphereCollider& sphere, const glm::vec3& v0, const glm::vec3& v1,
                                       const glm::vec3& v2);

  /**
   * @brief Finds the closest point on a triangle to a given point.
   *
   * This function uses the algorithm from "Real-Time Collision Detection" by Christer Ericson
   * to compute the closest point on the triangle defined by vertices a, b, and c to the point.
   *
   * @param point The point to find the closest point to.
   * @param a First vertex of the triangle.
   * @param b Second vertex of the triangle.
   * @param c Third vertex of the triangle.
   * @return The closest point on the triangle to the given point.
   */
  static glm::vec3 closestPointOnTriangle(const glm::vec3& point, const glm::vec3& a, const glm::vec3& b,
                                          const glm::vec3& c);

  /**
   * @brief Applies scaling, rotation, and translation transformations to a vertex.
   *
   * @param vertex The original vertex position.
   * @param transform The transformation to apply.
   * @return The transformed vertex position.
   */
  static glm::vec3 applyTransformation(const glm::vec3& vertex, const Mesh::Transform& transform);

  /**
   * @brief Rotates a vertex by given rotation angles.
   *
   * @param vertex The vertex to rotate.
   * @param rotation The rotation angles (in degrees) around the x, y, and z axes.
   * @return The rotated vertex.
   */
  static glm::vec3 rotateVertex(const glm::vec3& vertex, const glm::vec3& rotation);

  /**
  * @brief Calculates the normal vector of a collision.
  *
  * This function computes the normalized cross product of two edges of the triangle to determine the collision normal.
  *
  * @param closestPoint
  * @param sphereCenter
  * @return The normalized collision normal vector.
  */
  static glm::vec3 calculateCollisionNormal(const glm::vec3& closestPoint, const glm::vec3& sphereCenter);

  /**
  * @brief Calculates the penetration depth of a collision.
  *
  * This function determines how much the sphere has penetrated the triangle by computing the distance
  * between the sphere's center and the closest point on the triangle, then subtracting this distance from the sphere's radius.
  *
  * @param sphere The Sphere object representing the collider.
  * @param v0 The first vertex of the triangle.
  * @param v1 The second vertex of the triangle.
  * @param v2 The third vertex of the triangle.
  * @return The penetration depth as a float.
  */
  static float calculatePenetrationDepth(const SphereCollider& sphere, const glm::vec3& v0, const glm::vec3& v1,
                                         const glm::vec3& v2);
 };



}
#endif //COLLISION_H
