/**
* @file AssimpImporterFacade.h
 * @brief Provides a facade for importing models using the Assimp library.
 *
 * The AssimpImporterFacade class simplifies the process of importing 3D models
 * and extracting relevant data for use in the engine. It provides methods to load
 * models, configure renderable components, and handle embedded texture data.
 *
 * @details
 * - `loadModel`: Loads a model from a file and extracts mesh data.
 * - `setupMesh`: Configures a RenderableComponent using the loaded mesh data.
 * - `fetchEmbeddedTextureData`: (Commented out) Intended to extract raw texture data from embedded textures.
 *
 * @author Shaun Matthews
 * @date Created: 11/04/2025
 */

#ifndef ASSIMPIMPORTERFACADE_H
#define ASSIMPIMPORTERFACADE_H

#include <string>
#include <vector>

#include "Components/DataTypes.h"         
#include "Components/RenderableComponent.h"
#include "Interfaces/ITexture.h"
#include "Texture/ImageData.h"

/**
 * @class AssimpImporterFacade
 * @brief A facade class for importing models using the Assimp library.
 *
 * The AssimpImporterFacade class provides an interface for loading 3D models,
 * extracting mesh data, and configuring renderable components. It abstracts
 * the complexities of the Assimp library to simplify model importing.
 */
class AssimpImporterFacade {
public:

    /**
    * @brief Default constructor for the AssimpImporterFacade.
    */
    AssimpImporterFacade();

    /**
     * @brief Destructor for the AssimpImporterFacade.
     */
    ~AssimpImporterFacade();

    /**
     * @brief Load a model from the given file path.
     *
     * This method loads a 3D model from the specified file path and extracts
     * its mesh data into a vector of RawMeshData structures.
     *
     * @param filepath The path to the model file.
     * @param meshes A reference to a vector where the extracted mesh data will be stored.
     * @return True if the model was successfully loaded, false otherwise.
     */
    bool loadModel(const std::string& filepath, std::vector<RawMeshData>& meshes);

    /**
     * @brief Configure a RenderableComponent using loaded mesh data.
     *
     * This method sets up a RenderableComponent with the mesh data extracted
     * from a previously loaded model.
     *
     * @param renderableComponent The RenderableComponent to configure.
     */
    void setupMesh(RenderableComponent& renderableComponent);


private:
    /**
     * @brief Internal instance of the Assimp importer.
     *
     * This member variable holds a pointer to the Assimp importer instance
     * used for loading and processing models.
     */
    class AssimpImporter* m_importer;  
};


#endif //ASSIMPIMPORTERFACADE_H
