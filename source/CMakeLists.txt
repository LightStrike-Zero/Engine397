# CMake configuration for source files

# Source files go here
set(SOURCE_FILES
#       # Starts with Engine and goes down

        ## ENGINE
        # Actors
        Engine/Actors/Camera.cpp
        Engine/Actors/Material.cpp
        Engine/Actors/Mesh.cpp
        Engine/Actors/Texture.cpp
        Engine/Actors/TextureManager.cpp

        # Renderer
        Engine/Renderer/Renderer.cpp
        # Shaders
        Engine/Shaders/Shader.cpp

        # Main
        main.cpp

        Engine/Importers/AssimpImporter.cpp
        Engine/Importers/AssimpImporter.h
        Engine/Importers/ModelLoader.cpp
        Engine/Importers/ModelLoader.h
        Engine/Actors/MeshData.h
        Engine/Actors/MaterialData.h
        Engine/Actors/ModelData.h
        Engine/Actors/TextureLoader.cpp
        Engine/Actors/TextureLoader.h
        Engine/Renderer/ShadowMap.cpp
        Engine/Renderer/ShadowMap.h
        Engine/Actors/Lights/Light.h
        Engine/Actors/Scene.cpp
        Engine/Actors/Scene.h
        Engine/Components/MeshComponent.h
        Engine/Components/TransformComponent.h
        Engine/Components/MaterialComponent.h
)

# Add ImGUI source files
set(SOURCE_FILES ${SOURCE_FILES} ${IMGUI_SOURCES}
        Engine/Renderer/Framebuffer.cpp
        Engine/Renderer/Framebuffer.h
        Engine/Shaders/ShaderManager.cpp
        Engine/Shaders/ShaderManager.h
        Engine/Actors/Lights/PointLight.cpp
        Engine/Actors/Lights/PointLight.h
        Engine/Actors/Lights/DirectionalLight.cpp
        Engine/Actors/Lights/DirectionalLight.h
        Engine/Actors/Lights/Light.cpp)

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
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${Assimp_INCLUDE_DIRS} ${STB_Image_INCLUDE_DIRS} ${Entt_INCLUDE_DIRS} ${ImGUI_INCLUDE_DIRS} headers)

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