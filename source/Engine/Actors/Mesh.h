/**
 * @file Mesh.h
 * @brief Defines the Mesh class used for loading and rendering 3D mesh data from OBJ files.
 *
 * This file contains the declaration of the Mesh class, which is responsible for
 * importing 3D models from OBJ files and rendering them using OpenGL. 
 *
 * @author Shaun Matthews
 * @date Created: 3/09/2024
 * @date Modified:
 */


#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include "Material.h"
// #include "../Renderer/VertexArray.h"
// #include "../Renderer/IndexBuffer.h"
// #include "../Renderer/VertexBuffer.h"
// #include "../Renderer/VertexBufferLayout.h"


#include <vector>
#include <string>
#include <memory>
#include <glm/vec3.hpp>


/**
  * @brief The Mesh class represents a 3D model loaded from an OBJ file.
  *
  * The Mesh class stores vertex, normal, and texture coordinate data, along with face
  * information for rendering a 3D model. It also handles transformations and provides
  * methods to load a mesh from an OBJ file and render it using OpenGL.
  */
class Mesh {
public:

    struct VertexData
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;

        bool operator<(const VertexData& other) const {
            if (Position.x != other.Position.x) return Position.x < other.Position.x;
            if (Position.y != other.Position.y) return Position.y < other.Position.y;
            if (Position.z != other.Position.z) return Position.z < other.Position.z;

            if (Normal.x != other.Normal.x) return Normal.x < other.Normal.x;
            if (Normal.y != other.Normal.y) return Normal.y < other.Normal.y;
            if (Normal.z != other.Normal.z) return Normal.z < other.Normal.z;

            if (TexCoords.x != other.TexCoords.x) return TexCoords.x < other.TexCoords.x;
            return TexCoords.y < other.TexCoords.y;
        }
    };

        /**
         * @brief represents a vertex normal in 3D space.
         */
    struct Normal { float nx, ny, nz; };

        /**
         * @brief represents texture coordinates.
         * Ideally, this should be clamped between 0 and 1 - but I didnt have time.
         */
    struct TexCoord { float u, v; };

         /**
         * @brief represents a face in the 3D model.
         * 
         */
    struct Face {
        std::vector<int> vertexIndices;
        std::vector<int> normalIndices;
        std::vector<int> texCoordIndices;
    };

         /**
         * @brief A structure representing the transformation of the mesh in 3D space.
         * 
         * This includes the position, rotation (degrees), and scale of the mesh.
         */
    struct Transform {
        glm::vec3 position{ 0.0f, 0.0f, 0.0f };
        glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };
        glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
    };


    std::vector<glm::vec3> vertices;                ///< The list of vertices in the mesh.
    std::vector<Normal> normals;                    ///< The list of normals in the mesh.
    std::vector<TexCoord> texCoords;                ///< The list of texture coordinates in the mesh.
    std::vector<Face> faces;                        ///< The list of faces in the mesh.
    Transform transform;                            ///< The transformation data for the mesh.

    std::vector<VertexData> m_interleavedVertices;
    std::vector<unsigned int> m_interleavedIndices;

    // VertexArray m_VAO;                              ///< The vertex array object for the mesh.
    // VertexBuffer m_VBO;                             ///< The vertex buffer object for the mesh.
    // IndexBuffer m_IBO;                              ///< The index buffer object for the mesh.

         /**
         * @brief Default constructor for the Mesh class.
         * 
         * Initializes an empty mesh.
         */
    Mesh();

         /**
         * @brief Constructs a Mesh object and loads an OBJ file.
         * 
         * @param filename The path to the OBJ file to load.
         */
    // explicit Mesh(const std::string& filename);

    bool init(const std::string& filename);

    void generateInterleavedData();
    void correctNormals();

    std::vector<VertexData> getInterleavedVertices() const { return m_interleavedVertices; }
    std::vector<unsigned int> getInterleavedIndices() const { return m_interleavedIndices; }


        /**
         * @brief Imports an OBJ file and parses its contents.
         *
         * This method reads the provided OBJ file, extracting vertex positions, normals,
         * texture coordinates, and face indices. The mesh data is stored in the respective
         * vectors.
         *
         * @param filePath The path to the OBJ file to import.
         * @return true If the file was successfully imported.
         * @return false If the file could not be opened or read.
         */
    bool importOBJ(const std::string& filePath);

    // const VertexArray& getVertexArray() const { return m_VAO; }
    // const IndexBuffer& getIndexBuffer() const { return m_IBO; }

    // void processVertexData();
    // void setupMesh();

        /**
         * @brief Retrieves the center point of the mesh, not currently needed
         *
         * This accessor returns the center point of the mesh, which is useful
         * for certain calculations or transformations.
         *
         * @return const Vec3& The center point of the mesh.
         */
    const glm::vec3& getCenter() const;

        /**
         * @brief Retrieves the radius of the mesh's bounding sphere.
         *
         * This accessor returns the radius of the mesh, which is useful for
         * collision detection or bounding volume calculations.
         *
         * @return float The radius of the mesh's bounding sphere.
         */
    float getRadius() const;

    // /**
    //  * @brief Renders the 3D mesh using OpenGL, with optional transformations.
    //  *
    //  * This method binds the provided texture and applies the mesh's transformation
    //  * (translation, rotation, scaling) before rendering each face in the mesh.
    //  *
    //  */
    // void Draw(const Material& mat, bool useTexture) const;

        /**
         * @brief Sets the position of the mesh.
         *
         * This method sets the position of the mesh in 3D space.
         *
         * @param x The x-coordinate of the new position.
         * @param y The y-coordinate of the new position.
         * @param z The z-coordinate of the new position.
         */
    void setPosition(float x, float y, float z);

        /**
         * @brief Sets the rotation of the mesh.
         *
         * This method sets the rotation of the mesh around the x, y, and z axes.
         *
         * @param rotX The rotation around the x-axis in degrees.
         * @param rotY The rotation around the y-axis in degrees.
         * @param rotZ The rotation around the z-axis in degrees.
         */
    void setRotation(float rotX, float rotY, float rotZ);

    glm::mat4 getRotationMatrix() const;

        /**
         * @brief Sets the scale of the mesh.
         *
         * This method sets the scaling factors for the mesh along the x, y, and z axes.
         *
         * @param scaleX The scaling factor along the x-axis.
         * @param scaleY The scaling factor along the y-axis.
         * @param scaleZ The scaling factor along the z-axis.
         */
    void setScale(float scaleX, float scaleY, float scaleZ);

        /**
         * @brief Retrieves the position of the mesh.
         *
         * This accessor returns the current position of the mesh in 3D space.
         *
         * @return const Vec3& The position of the mesh.
         */
    const glm::vec3& getPosition() const;

        /**
         * @brief Retrieves the rotation of the mesh.
         *
         * This accessor returns the current rotation of the mesh in degrees around
         * the x, y, and z axes.
         *
         * @return const Vec3& The rotation of the mesh.
         */
    const glm::vec3& getRotation() const;

        /**
         * @brief Retrieves the scale of the mesh.
         *
         * This accessor returns the current scaling factors of the mesh along the x, y,
         * and z axes.
         *
         * @return const Vec3& The scale of the mesh.
         */
    const glm::vec3& getScale() const;

private:
    /// The center point of the mesh.
    glm::vec3 center{};
    /// The radius of the mesh's bounding sphere.
    float radius{};

};

#endif // MESH_H
