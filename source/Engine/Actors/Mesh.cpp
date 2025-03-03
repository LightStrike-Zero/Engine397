/**
 * @file Mesh.h
 * @brief Defines the Mesh class used for loading and rendering 3D mesh data from OBJ files.
 *
 * @author Shaun Matthews
 * @date Created: 3/09/2024
 * @date Modified: 
 */

#include "Mesh.h"
#include "Material.h"

#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <glm/ext/matrix_transform.hpp>

#include "chrono"

 /**
  * @brief Constructs
  *
  * @param
  */
Mesh::Mesh(){
}

/**
 * @brief
 *
 *
 * @param
 * @throw std::runtime_error If the OBJ file fails to load.
 */
bool Mesh::init(const std::string& filename) {
    if (importOBJ(filename)) {
        // processVertexData();
        // setupMesh();
        return true;
    } else {
        std::cerr << "Failed to load OBJ file: " << filename << std::endl;
        return false;
    }
}

void Mesh::generateInterleavedData()
{
    // Clear existing interleaved data to avoid duplication
    m_interleavedVertices.clear();
    m_interleavedIndices.clear();

    // Map to track unique vertices and assign them an index
    std::map<VertexData, unsigned int> uniqueVertices;

    for (const auto& face : faces) {
        for (size_t i = 0; i < face.vertexIndices.size(); ++i) {
            VertexData vertex;
            vertex.Position = vertices[face.vertexIndices[i]];
            vertex.Normal = glm::vec3(normals[face.normalIndices[i]].nx,
                                      normals[face.normalIndices[i]].ny,
                                      normals[face.normalIndices[i]].nz);
            vertex.TexCoords = glm::vec2(texCoords[face.texCoordIndices[i]].u,
                                        texCoords[face.texCoordIndices[i]].v);

            // Check if the vertex is already in the map
            if (uniqueVertices.find(vertex) == uniqueVertices.end()) {
                uniqueVertices[vertex] = static_cast<unsigned int>(m_interleavedVertices.size());
                m_interleavedVertices.push_back(vertex);
            }

            // Add the index of the vertex to the index buffer
            m_interleavedIndices.push_back(uniqueVertices[vertex]);
        }
    }

    // Optional: Verify that interleaved data was generated correctly
    if (m_interleavedVertices.empty() || m_interleavedIndices.empty()) {
        throw std::runtime_error("Failed to generate interleaved vertex data.");
    }
}

void Mesh::correctNormals()
{
    for (auto& face : faces)
    {
        glm::vec3 v0 = vertices[face.vertexIndices[0]];
        glm::vec3 v1 = vertices[face.vertexIndices[1]];
        glm::vec3 v2 = vertices[face.vertexIndices[2]];

        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 geometricNormal = glm::normalize(glm::cross(edge1, edge2));

        glm::vec3 storedNormal = glm::normalize(glm::vec3(
            normals[face.normalIndices[0]].nx,
            normals[face.normalIndices[0]].ny,
            normals[face.normalIndices[0]].nz
        ));

        float dotProduct = glm::dot(geometricNormal, storedNormal);

        if (dotProduct < 0.0f)
        {
            for (int normalIndex : face.normalIndices)
            {
                normals[normalIndex].nx = -normals[normalIndex].nx;
                normals[normalIndex].ny = -normals[normalIndex].ny;
                normals[normalIndex].nz = -normals[normalIndex].nz;
            }

            std::reverse(face.vertexIndices.begin(), face.vertexIndices.end());
        }
    }
}

/**
 * @brief Imports an OBJ file and parses its contents.
 *
 * This method reads the provided OBJ file, extracting vertex positions, normals,
 * texture coordinates, and face indices. The mesh data is stored in the respective
 * vectors.
 * 
 * NOTE: I assumed the OBJ file only contains ONE object/model
 *
 * @param filePath The path to the OBJ file to import.
 * @return true If the file was successfully imported.
 * @return false If the file could not be opened or read.
 */
bool Mesh::importOBJ(const std::string& filePath) 
{
    using namespace std::chrono;

    auto start = high_resolution_clock::now();

    std::ifstream file(filePath);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            // Parse and store vertex positions
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
        else if (prefix == "vn") {
            // Parse and store vertex normals
            Normal normal;
            iss >> normal.nx >> normal.ny >> normal.nz;
            normals.push_back(normal);
        }
        else if (prefix == "vt") {
            // Parse and store texture coordinates
            TexCoord texCoord;
            iss >> texCoord.u >> texCoord.v;
            texCoords.push_back(texCoord);
        }
        else if (prefix == "f") {
            // Parse faces
            std::vector<int> vertexIndices, texCoordIndices, normalIndices;
            std::string vertexData;

            // Read each vertex/texture/normal index triplet in the face
            while (iss >> vertexData) {
                std::istringstream vertexStream(vertexData);
                std::string vertexIndex, texCoordIndex, normalIndex;

                std::getline(vertexStream, vertexIndex, '/');
                std::getline(vertexStream, texCoordIndex, '/');
                std::getline(vertexStream, normalIndex, '/');

                vertexIndices.push_back(std::stoi(vertexIndex) - 1);
                if (!texCoordIndex.empty()) {
                    texCoordIndices.push_back(std::stoi(texCoordIndex) - 1);
                }
                if (!normalIndex.empty()) {
                    normalIndices.push_back(std::stoi(normalIndex) - 1);
                }
            }

            // Triangulate the face if it has more than 3 vertices
            for (size_t i = 1; i < vertexIndices.size() - 1; ++i) {
                Face face;
                face.vertexIndices = { vertexIndices[0], vertexIndices[i], vertexIndices[i + 1] };

                if (!texCoordIndices.empty()) {
                    face.texCoordIndices = { texCoordIndices[0], texCoordIndices[i], texCoordIndices[i + 1] };
                }
                if (!normalIndices.empty()) {
                    face.normalIndices = { normalIndices[0], normalIndices[i], normalIndices[i + 1] };
                }

                faces.push_back(face);
            }
        }
    }

    correctNormals();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    std::cout << "Time taken to load OBJ: " << duration.count() << " milliseconds" << "\tname: " << filePath << std::endl;

    file.close();
    return true;
}

// void Mesh::processVertexData() {
//     std::unordered_map<std::string, unsigned int> uniqueVertices;
//
//     for (const auto& face : faces) {
//         for (size_t i = 0; i < face.vertexIndices.size(); ++i) {
//             std::ostringstream keyStream;
//             keyStream << face.vertexIndices[i] << "/"
//                       << (face.texCoordIndices.empty() ? 0 : face.texCoordIndices[i])
//                       << "/"
//                       << (face.normalIndices.empty() ? 0 : face.normalIndices[i]);
//             std::string key = keyStream.str();
//
//             // Check if the vertex is unique
//             if (uniqueVertices.find(key) == uniqueVertices.end()) {
//                 VertexData vertex;
//
//                 // Use the indices directly without adjustment if already 0-based
//                 int vertexIndex = face.vertexIndices[i];
//                 if (vertexIndex >= 0 && vertexIndex < vertices.size()) {
//                     vertex.Position = vertices[vertexIndex];
//                 } else {
//                     std::cerr << "Error: Vertex index out of bounds: " << vertexIndex << std::endl;
//                     continue;
//                 }
//
//                 // Similar checks for normals and texCoords
//                 if (!normals.empty() && !face.normalIndices.empty()) {
//                     int normalIndex = face.normalIndices[i];
//                     if (normalIndex >= 0 && normalIndex < normals.size()) {
//                         vertex.Normal = glm::vec3(normals[normalIndex].nx,
//                                                   normals[normalIndex].ny,
//                                                   normals[normalIndex].nz);
//                     } else {
//                         vertex.Normal = glm::vec3(0.0f, 0.0f, 0.0f);
//                     }
//                 } else {
//                     vertex.Normal = glm::vec3(0.0f, 0.0f, 0.0f);
//                 }
//
//                 if (!texCoords.empty() && !face.texCoordIndices.empty()) {
//                     int texCoordIndex = face.texCoordIndices[i];
//                     if (texCoordIndex >= 0 && texCoordIndex < texCoords.size()) {
//                         vertex.TexCoords = glm::vec2(texCoords[texCoordIndex].u,
//                                                      texCoords[texCoordIndex].v);
//                     } else {
//                         vertex.TexCoords = glm::vec2(0.0f, 0.0f);
//                     }
//                 } else {
//                     vertex.TexCoords = glm::vec2(0.0f, 0.0f);
//                 }
//
//                 flattenedVertexData.push_back(vertex);
//                 uniqueVertices[key] = static_cast<unsigned int>(flattenedVertexData.size() - 1);
//             }
//
//             indices.push_back(uniqueVertices[key]);
//         }
//     }
// }



// void Mesh::setupMesh()
// {
//     // Initialize Vertex Array
//     m_VAO.Bind();
//
//     // Initialize Vertex Buffer with flattened vertex data
//     m_VBO = VertexBuffer(flattenedVertexData.data(), flattenedVertexData.size() * sizeof(VertexData));
//
//     // Define the layout of the vertex data
//     VertexBufferLayout layout;
//     layout.Push<float>(3); // Position
//     layout.Push<float>(3); // Normal
//     layout.Push<float>(2); // TexCoords
//
//     // Add buffer to Vertex Array
//     m_VAO.AddBuffer(m_VBO, layout);
//
//     // Initialize Index Buffer
//     m_IBO = IndexBuffer(indices.data(), indices.size());
//
//     // Unbind for cleanliness
//     m_VAO.Unbind();
//     m_VBO.Unbind();
//     // m_IBO.Unbind();
// }

/**
 * @brief Renders the 3D mesh using OpenGL
 *
 * This method binds the provided texture and applies the mesh's transformation
 * (translation, rotation, scaling) before rendering each face in the mesh.
 *
 * @param textureID The OpenGL texture ID to bind before rendering.
 */
// void Mesh::Draw(const Material& mat, bool useTexture) const
// {
//     glEnable(GL_LIGHTING);
//
//     // apply material
//     glMaterialfv(GL_FRONT, GL_AMBIENT, mat.Ka);
//     // Set diffuse color
//     glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.Kd);
//     // Set specular color
//     glMaterialfv(GL_FRONT, GL_SPECULAR, mat.Ks);
//     // Set emissive color (if any)
//     glMaterialfv(GL_FRONT, GL_EMISSION, mat.Ke);
//     // Set shininess
//     glMaterialf(GL_FRONT, GL_SHININESS, mat.Ns);
//
//     // if material has transparancy, enable blending
//     if (mat.d < 1.0f) {
//         glEnable(GL_BLEND);
//         // blend the source alpha with the inverse of the destination alpha
//         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//     }
//     else {
//         glDisable(GL_BLEND);
//     }
//
//
//
//     // Bind the texture
//     glBindTexture(GL_TEXTURE_2D, m_textureID);
//     glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  // Modulate texture with lighting
//     glEnable(GL_TEXTURE_2D);
//
//     // Enable depth testing
//     glEnable(GL_DEPTH_TEST);
//     glDepthFunc(GL_LEQUAL);
//
//     glPushMatrix();
//
//     // Retrieve transformation values
//     const Vec3& position = getPosition();
//     const Vec3& rotation = getRotation();
//     const Vec3& scale = getScale();
//
//     // Apply transformation, rotation & scale.
//     glTranslatef(position.x, position.y, position.z);
//     glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
//     glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
//     glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
//     glScalef(scale.x, scale.y, scale.z);
//
//     // Draw the mesh
//     for (const auto& face : faces) {
//         glBegin(GL_TRIANGLES);
//
//         for (size_t i = 0; i < face.vertexIndices.size(); ++i) {
//             int vertexIndex = face.vertexIndices[i];
//             const Vertex& vertex = vertices[vertexIndex];
//
//             // specify the texture coordinates
//             if (i < face.texCoordIndices.size()) {
//                 int texCoordIndex = face.texCoordIndices[i];
//                 const TexCoord& texCoord = texCoords[texCoordIndex];
//                 glTexCoord2f(texCoord.u, texCoord.v);
//             }
//             // specify normals
//             if (i < face.normalIndices.size()) {
//                 int normalIndex = face.normalIndices[i];
//                 const Normal& normal = normals[normalIndex];
//                 glNormal3f(normal.nx, normal.ny, normal.nz);
//             }
//
//             // specify the vertex position
//             glVertex3f(vertex.x, vertex.y, vertex.z);
//         }
//
//         glEnd();
//     }
//
//     glPopMatrix();
//     glDisable(GL_TEXTURE_2D);
//
//     if (mat.d < 1.0f) {
//         glDisable(GL_BLEND);
//     }
//
//     // turn the lights off so we don't upset master shay (dont want to light up shays world)
//     glDisable(GL_LIGHTING);
// }



/**
 * @brief Gets the center of the mesh.
 *
 * @return The center vertex of the mesh.
 */
const glm::vec3& Mesh::getCenter() const {
    return center;
}

/**
 * @brief Gets the radius of the mesh's bounding sphere.
 *
 * @return The radius of the mesh's bounding sphere.
 */
float Mesh::getRadius() const {
    return radius;
}

/**
 * @brief Gets the current position of the mesh.
 *
 * @return The position of the mesh.
 */
const glm::vec3& Mesh::getPosition() const {
    return transform.position;
}

/**
 * @brief Gets the current rotation of the mesh.
 *
 * @return The rotation of the mesh.
 */
const glm::vec3& Mesh::getRotation() const {
    return transform.rotation;
}

/**
 * @brief Gets the current scale of the mesh.
 *
 * @return The scale of the mesh.
 */
const glm::vec3& Mesh::getScale() const {
    return transform.scale;
}

/**
 * @brief Sets the position of the mesh.
 *
 * @param x The X-coordinate of the new position.
 * @param y The Y-coordinate of the new position.
 * @param z The Z-coordinate of the new position.
 */
void Mesh::setPosition(float x, float y, float z) {
    transform.position = { x, y, z };
}

/**
 * @brief Sets the rotation of the mesh.
 *
 * @param rotX The rotation angle around the X-axis.
 * @param rotY The rotation angle around the Y-axis.
 * @param rotZ The rotation angle around the Z-axis.
 */
void Mesh::setRotation(float rotX, float rotY, float rotZ) {
    transform.rotation = { rotX, rotY, rotZ };
}

/**
 * @brief Sets the scale of the mesh.
 *
 * @param scaleX The scaling factor along the X-axis.
 * @param scaleY The scaling factor along the Y-axis.
 * @param scaleZ The scaling factor along the Z-axis.
 */
void Mesh::setScale(float scaleX, float scaleY, float scaleZ) {
    transform.scale = { scaleX, scaleY, scaleZ };
}


