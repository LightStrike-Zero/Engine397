#include <iostream>

#include "GUI/ImGui_UI.h"
#include "Scene.h"
#include "Factorys/RendererFactory.h"
#include "Interfaces/IWindow.h"
#include "Terrain/TerrainFactory.h"
#include "Systems/CameraSystem.h"
#include "Systems/GridCollision.h"
#include "Terrain/TerrainMenuHelper.h"
#include "Window/glfwWindow.h"

int main(int argc, char** argv)
{
    
    int terrainGridRows = 1000, terrainGridCols = 1000;
    float terrainScale = 0.1f;

    TerrainTypeEnum chosenType;
    std::map<std::string, std::string> chosenParams;
    getTerrainMenu(chosenType, chosenParams);

    float playerHeight = 1.0f;

    IWindow* window = new GLFWWindow(1920, 1080, "Lab 4", true);
    int windowWidth, windowHeight;
    window->GetFramebufferSize(windowWidth, windowHeight);
    float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    window->SetInputMode(CURSOR, CURSOR_NORMAL);

    // Select the OpenGL API renderer
    const RendererType type = RendererType::OpenGL;
    IRenderer* renderer = RendererFactory::CreateRenderer(type);

    ImGuiUI Gui;
    Gui.Initialise(static_cast<GLFWwindow*>(window->GetNativeWindow()));
    
    
    /*
     * This is setting the paths to the current models available
     * TODO move this to a config file - ideally a lua file
     * Scene is also established here, and models loaded in
     */
    // std::string backPackPath = (R"(Assets\survival_guitar_backpack_scaled\scene.gltf)");
    // std::string sponzaPath = (R"(Assets\main1_sponza\NewSponza_Main_glTF_003.gltf)");

    
    Scene scene;
    DirectionalLight dirLight(glm::vec3(-0.5f, -0.9f, -0.5f), glm::vec3(0.3f, 0.3f, 0.3f),
        glm::vec3(1.0f, 0.99f, 0.99f), glm::vec3(0.09f, 0.09f, 0.09f), 0.05f);
    
    scene.setDirectionalLight(dirLight);
    // scene.loadModelToRegistry(backPackPath);

    auto cameraEntity = scene.getRegistry().create();
    scene.getRegistry().emplace<TransformComponent>(cameraEntity, glm::vec3(0.0f, 20.0f, 0.0f)); // this set the player/camera start pos
    scene.getRegistry().emplace<CameraComponent>(cameraEntity);
    CameraSystem cameraSystem(static_cast<GLFWwindow*>(window->GetNativeWindow()), aspectRatio);

    auto terrainType = TerrainFactory::createTerrainType(chosenType, chosenParams);
    Terrain terrain(terrainGridRows, terrainGridCols, terrainScale);
    terrain.setTerrainType(terrainType);
    terrain.generateTerrain();
    scene.addTerrainToScene(terrain);

    const RawMeshData& terrainMeshData = terrain.getMeshData();
    GridCollision collision(terrainGridRows, terrainGridCols, terrainScale, terrainMeshData.vertices);

    static float lastFrame = 0.0f;

    while (!window->ShouldClose())
    {
        float currentFrame = window->GetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // camera system 
        cameraSystem.update(scene.getRegistry(), deltaTime);
        auto [viewMatrix, projectionMatrix, viewPos] = cameraSystem.getActiveCameraMatrices(scene.getRegistry());

        // terrian collision
        // comment this out to disable terrain collision
        auto* playerTransform = scene.getRegistry().try_get<TransformComponent>(cameraEntity);
        glm::vec3 playerPos = playerTransform->position;
        float terrainHeight = collision.getHeightAt(playerPos);
        playerTransform->position.y = terrainHeight + playerHeight;
        
        Gui.BeginFrame();
        Gui.DisplayImage("Viewport", renderer->Render(scene, viewMatrix, projectionMatrix, viewPos), glm::vec2{windowWidth, windowHeight});
        Gui.EndFrame();

        window->SwapBuffers();
        window->PollEvents();
    }

    Gui.Shutdown();
    delete window;
    return 0;
}
