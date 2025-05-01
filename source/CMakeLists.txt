# CMake configuration for source files

# Source files go here
set(SOURCE_FILES
        Engine/Texture/TextureManager.cpp
        Engine/Shaders/Shader.cpp
        main.cpp
        Engine/Importers/AssimpImporter.cpp
        Engine/Importers/AssimpImporter.h
        Engine/Importers/ModelLoader.cpp
        Engine/Importers/ModelLoader.h
        Engine/Texture/TextureLoader.cpp
        Engine/Texture/TextureLoader.h
        Engine/Actors/Lights/Light.h
        Engine/Components/TransformComponent.h
        Engine/Components/MaterialComponent.h
        Engine/GUI/ImGui_UI.cpp

)

# Add ImGUI source files
set(SOURCE_FILES ${SOURCE_FILES} ${IMGUI_SOURCES}
        Engine/Shaders/ShaderManager.cpp
        Engine/Shaders/ShaderManager.h
        Engine/Actors/Lights/DirectionalLight.cpp
        Engine/Actors/Lights/DirectionalLight.h
        Engine/Actors/Lights/Light.cpp
        Engine/Interfaces/IMeshBuffer.h
        Engine/OpenGL/OpenGLMeshBuffer.cpp
        Engine/OpenGL/OpenGLMeshBuffer.h
        Engine/Interfaces/IDataBuffer.h
        Engine/OpenGL/OpenGLFrameBuffer.cpp
        Engine/OpenGL/OpenGLFrameBuffer.h
        Engine/OpenGL/OpenGLShadowMapBuffer.cpp
        Engine/OpenGL/OpenGLShadowMapBuffer.h
        Engine/OpenGL/OpenGLQuadBuffer.cpp
        Engine/OpenGL/OpenGLQuadBuffer.h
        Engine/Interfaces/IRenderer.h
        Engine/OpenGL/OpenGLRenderer.cpp
        Engine/OpenGL/OpenGLRenderer.h
        Engine/Interfaces/IShader.h
        Engine/OpenGL/OpenGLShader.cpp
        Engine/OpenGL/OpenGLShader.h
        Engine/Factorys/RendererFactory.h
        Engine/Terrain/Terrain.cpp
        Engine/Terrain/Terrain.h
        Engine/Terrain/HeightmapTerrain.cpp
        Engine/Terrain/HeightmapTerrain.h
        Engine/Terrain/FractalTerrain.cpp
        Engine/Terrain/FractalTerrain.h
        Engine/Terrain/TerrainType.h
        Engine/Terrain/TerrainTexture.h
        Engine/Terrain/TerrainTexture.cpp
        Engine/Terrain/TerrainFactory.h
        Engine/Terrain/TerrainFactory.cpp
        Engine/Components/CameraComponent.h
        Engine/Systems/CameraSystem.cpp
        Engine/Systems/CameraSystem.h
        Engine/Systems/EventSystem.h
        Engine/Components/DataTypes.h
        Engine/Systems/GridCollision.cpp
        Engine/Systems/GridCollision.h
        Engine/Interfaces/IWindow.h
        Engine/Window/glfwWindow.cpp
        Engine/Window/glfwWindow.h
        Engine/Scripting/LuaManager.cpp
        Engine/Components/PlayerControllerComponent.h
        Engine/Actors/Player.cpp
        Engine/Actors/Player.h
        StuffThatNeedsToBeLoadedInLua.h
        Engine/ResourceManagement/Scene.cpp
        Engine/ResourceManagement/Scene.h
        Engine/Factorys/EntityFactory.cpp
        Engine/Factorys/EntityFactory.h
        Engine/ResourceManagement/EnttFacade.h
        Engine/Components/CollisionComponents/CollidableComponent.h
        Engine/Components/CollisionComponents/BoxColliderComponent.h
        Engine/Components/CollisionComponents/CapsuleColliderComponent.h
        Engine/Components/CollisionComponents/SphereColliderComponent.h
#        Engine/Physics_DEPRECIATED/TankCollision.h
        Engine/Physics_DEPRECIATED/TankCollision.cpp
        Engine/Utility/FileHandler.cpp
        Engine/Utility/FileHandler.h
        Engine/Importers/AssimpImporterFacade.cpp
        Engine/Importers/AssimpImporterFacade.h
        Engine/Interfaces/ITexture.h
        Engine/Texture/OpenGL/OGL_Texture.cpp
        Engine/Texture/OpenGL/OGL_Texture.h
        Engine/Texture/ImageData.h
        Engine/Factorys/TextureFactory.cpp
        Engine/Factorys/TextureFactory.h
        Engine/Interfaces/ICubeMap.h
        Engine/Texture/OpenGL/OGL_CubeMap.h
        Engine/Texture/OpenGL/OGL_CubeMap.cpp
        Engine/Components/SkyboxComponent.h
        Engine/Components/NameComponent.h
        Engine/Terrain/TerrainMultiTexture.cpp
        Engine/Terrain/TerrainMultiTexture.h
        Engine/ResourceManagement/AssetRegistry.cpp
        Engine/ResourceManagement/AssetRegistry.h
        Engine/ResourceManagement/PrototypeRegistry.h
)

# Add executable based on source files
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Define GLEW_STATIC to link with the static GLEW library along with ImGUI OpenGL loader
target_compile_definitions(${PROJECT_NAME} PRIVATE GLEW_STATIC IMGUI_IMPL_OPENGL_LOADER_GLEW)

# Include directories for headers to simplify include paths
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Engine
        ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Actors
        ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Renderer
        ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Shaders
        ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Utility
        ${CMAKE_CURRENT_SOURCE_DIR}/Engine/Interaction
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/ImGUI
        
)

# Link libraries
if (WIN32)
    # Windows-specific linking
    target_link_libraries(${PROJECT_NAME}
            OpenGL::GL
            ${GLFW_LIBRARIES}
            ${GLEW_LIBRARIES}
            ${Assimp_LIBRARIES}
            ${LUA_LIBRARIES}
            opengl32.lib
            glu32.lib
            user32.lib
            gdi32.lib
            winmm.lib
            setupapi.lib
            hid.lib
            imm32.lib
            version.lib
    )
elseif (UNIX)
    # Unix/Linux-specific linking
    target_link_libraries(${PROJECT_NAME}
            OpenGL::GL              # Equivalent of opengl32.lib
            GLU                     # Equivalent of glu32.lib
            X11                     # Equivalent of user32.lib
            asound                  # Equivalent of winmm.lib
            udev                    # Equivalent of hid.lib
            ${GLFW_LIBRARIES}
            ${GLEW_LIBRARIES}
            ${Assimp_LIBRARIES}
    )
endif()
# Include directories for header files
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${Assimp_INCLUDE_DIRS} ${STB_Image_INCLUDE_DIRS} ${Entt_INCLUDE_DIRS} ${ImGUI_INCLUDE_DIRS} ${JSON_INCLUDE_DIRS} headers)

# Additional dependencies for static linking on Windows
if (WIN32)
    target_link_libraries(${PROJECT_NAME})
endif()


# Post-build command to copy the Assimp DLL into the build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/assimp/bin/Release/assimp-vc143-mt.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

