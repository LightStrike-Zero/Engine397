/**
* @file Collision.cpp
 * @author Shaun Matthews
 * @date 21/09/2024
 * @brief Implementation of the Physics class responsible for collision detection and resolution.
 */

#include "Collision.h"
#include "Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <chrono>
#include <iostream>

#include "Octree.h"
// #include "DebugRenderer.h"

const float EPSILON = 1e-5f; // Small threshold value


namespace MinPhysics
{
    int Collision::collisionTestCounter = 0;


    bool Collision::checkCollisions(Camera& cam, OctreeNode* octreeRoot)
{
    using namespace std::chrono;
    auto startPhysicsClock = high_resolution_clock::now();

    bool collisionDetected = false;
    std::vector<CollisionInfo> collisions;

    // Retrieve potential colliders using the camera's SphereCollider
    SphereCollider cameraCollider(cam.getPosition(), cam.getCollider().radius);
    std::vector<BaseActor*> potentialColliders;
    octreeRoot->retrieve(cameraCollider, potentialColliders);

    // Reset collision flags at the start of each check
    for (BaseActor* object : potentialColliders)
    {
        object->setColliding(false);
    }

        // Reset grounded state
        cam.setGrounded(false);

    // Perform face-level collision detection
    for (BaseActor* object : potentialColliders)
    {
        const auto& mesh = object->getMesh();
        const auto& vertices = mesh.vertices;
        const auto& faces = mesh.faces;
        const auto& transform = mesh.transform;

        for (const auto& face : faces)
        {
            // Get the vertex indices for this face
            int idx0 = face.vertexIndices[0];
            int idx1 = face.vertexIndices[1];
            int idx2 = face.vertexIndices[2];

            // Get the vertices of the triangle
            glm::vec3 v0 = vertices[idx0];
            glm::vec3 v1 = vertices[idx1];
            glm::vec3 v2 = vertices[idx2];

            // Apply the object's transformation to the vertices
            v0 = applyTransformation(v0, transform);
            v1 = applyTransformation(v1, transform);
            v2 = applyTransformation(v2, transform);

            if (sphereIntersectsTriangle(cameraCollider, v0, v1, v2))
            {
                // Calculate the closest point on the triangle to the sphere center
                glm::vec3 closestPoint = closestPointOnTriangle(cameraCollider.center, v0, v1, v2);

                // Calculate the penetration depth
                float distance = glm::distance(closestPoint, cameraCollider.center);
                float penetrationDepth = cameraCollider.radius - distance;

                // Check if penetration depth is positive
                if (penetrationDepth > EPSILON)
                {
                    CollisionInfo info;


                    // Calculate the collision normal pointing from the collision point to the sphere center
                    info.normal = glm::normalize(cameraCollider.center - closestPoint);;
                    info.penetrationDepth = penetrationDepth;
                    info.collidedObject = object;
                    info.v0 = v0;
                    info.v1 = v1;
                    info.v2 = v2;
                    info.closestPoint = closestPoint;

                    collisions.push_back(info);
                }
            }
        }
    }


        // Accumulate collision adjustments
        glm::vec3 totalAdjustment(0.0f);

        // Handle collisions
        for (const auto& collision : collisions)
        {
            collision.collidedObject->setColliding(true);

            // Adjust the camera's position
            if (collision.normal.y > 0.7f)
            {
                cam.addToPosition(glm::vec3(0.0f, collision.normal.y * collision.penetrationDepth, 0.0f));
                cam.setGrounded(true);
            }
            else
            {
                cam.addToPosition(collision.normal * collision.penetrationDepth);
            }

            // Adjust the camera's velocity
            float velocityIntoSurface = glm::dot(cam.getVelocity(), collision.normal);
            if (velocityIntoSurface < 0.0f)
            {
                glm::vec3 adjustment = collision.normal * velocityIntoSurface;
                cam.setVelocity(cam.getVelocity() - adjustment);
            }
            // Accumulate the collision adjustments
            // totalAdjustment += collision.normal * collision.penetrationDepth;

            // If the collision normal indicates ground contact
            if (collision.normal.y > 0.7f)
            {
                glm::vec3 velocity = cam.getVelocity();
                velocity.y = 0.0f;
                cam.setVelocity(velocity);
            }
            collisionDetected = true;
        }

        // Limit the total adjustment to prevent overcorrection
        // float maxAdjustment = 10.0f; // Adjust this value as appropriate for your application
        // if (glm::length(totalAdjustment) > maxAdjustment)
        // {
        //     totalAdjustment = glm::normalize(totalAdjustment) * maxAdjustment;
        // }

        // Apply the total adjustment to the camera position
        // cam.addToPosition(totalAdjustment);

        // Update the debug renderer with collided faces
        // DebugRenderer::getInstance().setCollidedFaces(collisions);

        auto endPhysicsClock = high_resolution_clock::now();
        auto totalPhysicsTime = duration_cast<milliseconds>(endPhysicsClock - startPhysicsClock).count();
        // std::cout << "Collision time for frame: " << totalPhysicsTime << " ms" << std::endl;

        return collisionDetected;
}


    void Collision::resolveCollision(Camera& cam, const glm::vec3& normal, float penetrationDepth)
    {
        if (penetrationDepth > 0.0f)
        {
            cam.addToPosition(normal * penetrationDepth);
        }
    }

    bool Collision::sphereIntersectsTriangle(const SphereCollider& sphere, const glm::vec3& v0, const glm::vec3& v1,
                                           const glm::vec3& v2)
    {
        glm::vec3 closestPoint = closestPointOnTriangle(sphere.center, v0, v1, v2);
        glm::vec3 diff = closestPoint - sphere.center;
        float distanceSquared = glm::dot(diff, diff);
        return distanceSquared <= sphere.radius * sphere.radius;
    }

    glm::vec3 Collision::closestPointOnTriangle(const glm::vec3& point, const glm::vec3& a, const glm::vec3& b,
                                              const glm::vec3& c)
    {
        // Using the algorithm from "Real-Time Collision Detection" by Christer Ericson

        // Compute vectors
        glm::vec3 ab = b - a;
        glm::vec3 ac = c - a;
        glm::vec3 ap = point - a;

        // Compute dot products
        float d1 = glm::dot(ab, ap);
        float d2 = glm::dot(ac, ap);

        // Check if point is in vertex region outside A
        if (d1 <= 0.0f && d2 <= 0.0f) return a;

        // Check if point is in vertex region outside B
        glm::vec3 bp = point - b;
        float d3 = glm::dot(ab, bp);
        float d4 = glm::dot(ac, bp);
        if (d3 >= 0.0f && d4 <= d3) return b;

        // Check if point is in edge region of AB
        float vc = d1 * d4 - d3 * d2;
        if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
        {
            float v = d1 / (d1 - d3);
            return a + ab * v;
        }

        // Check if point is in vertex region outside C
        glm::vec3 cp = point - c;
        float d5 = glm::dot(ab, cp);
        float d6 = glm::dot(ac, cp);
        if (d6 >= 0.0f && d5 <= d6) return c;

        // Check if point is in edge region of AC
        float vb = d5 * d2 - d1 * d6;
        if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
        {
            float w = d2 / (d2 - d6);
            return a + ac * w;
        }

        // Check if point is in edge region of BC
        float va = d3 * d6 - d5 * d4;
        if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
        {
            float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
            glm::vec3 bc = c - b;
            return b + bc * w;
        }

        // Point is inside face region. Compute Q through its barycentric coordinates (u, v, w)
        float denom = 1.0f / (va + vb + vc);
        float v = vb * denom;
        float w = vc * denom;
        return a + ab * v + ac * w;
    }

    // Helper function to apply transformations
    glm::vec3 Collision::applyTransformation(const glm::vec3& vertex, const Mesh::Transform& transform)
    {
        glm::vec3 scaled = vertex * transform.scale;
        glm::vec3 rotated = rotateVertex(scaled, transform.rotation);
        glm::vec3 transformed = rotated + transform.position;
        return transformed;
    }

    // Helper function to rotate a vertex
    glm::vec3 Collision::rotateVertex(const glm::vec3& vertex, const glm::vec3& rotation)
    {
        glm::vec3 rotRad = glm::radians(rotation);
        glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), rotRad.x, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), rotRad.y, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), rotRad.z, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 rotationMatrix = rotZ * rotY * rotX;
        glm::vec4 rotatedVertex = rotationMatrix * glm::vec4(vertex, 1.0f);
        return glm::vec3(rotatedVertex);
    }

    glm::vec3 Collision::calculateCollisionNormal(const glm::vec3& closestPoint, const glm::vec3& sphereCenter)
    {
        glm::vec3 normal = glm::normalize(sphereCenter - closestPoint);
        return normal;
    }

    float Collision::calculatePenetrationDepth(const SphereCollider& sphere, const glm::vec3& v0, const glm::vec3& v1,
                                             const glm::vec3& v2)
    {
        glm::vec3 closestPoint = closestPointOnTriangle(sphere.center, v0, v1, v2);
        float distance = glm::distance(closestPoint, sphere.center);
        return sphere.radius - distance;
    }
}
