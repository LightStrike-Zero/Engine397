#include <iostream>
#include <Scripting/LuaManager.h>
#include <Scripting/ScriptManager.h>

#include "GUI/ImGui_UI.h"
#include "Scene.h"
#include "Factorys/RendererFactory.h"
#include "Interfaces/IWindow.h"
#include "Terrain/TerrainFactory.h"
#include "Systems/CameraSystem.h"
#include "Systems/GridCollision.h"
#include "Terrain/TerrainMenuHelper.h"
#include "Window/glfwWindow.h"
// Buko -------------------------
// Libraries for scripting
#include <lua.hpp>
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "Components/PlayerControllerComponent.h"
//----------------------

int main(int argc, char** argv)
{
    
    int terrainGridRows = 1000, terrainGridCols = 1000;
    float terrainScale = 0.1f;


    TerrainTypeEnum chosenType = TerrainTypeEnum::TEXTURED_FRACTAL;
    std::map<std::string, std::string> chosenParams = {
        {"iterations", "250"},
        {"initialDisplacement", "1000.0f"},
        {"displacementDecay", "0.97f"},
        {"heightScale", "20.5f"},
        {"seed", std::to_string(4)},
        {"smoothness", std::to_string(0.8f)},
        {"smoothingPasses", "20"},
        {"texturePath", R"(Assets\Terrain\Textures\sand.png)"}
    };
    // getTerrainMenu(chosenType, chosenParams);

    float playerHeight = 2.0f;

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
    
    Scene scene;
    
    // --- Buko setting up Lua/Sol -------------------------
    /*
     * ScriptManager dynamic allocation using a pointer: flexibility, type of object can be changed at runtime
     * Lua script handles the asset paths
     * Lua script loads the models into the scene through a method in scene
     */
    //
    // ScriptManager* scriptManager = new LuaManager();      // Lua Manager instance is instantiated derived from base class
    //
    // scriptManager->registerScene(scene);                                // Expose Scene to Lua
    // scriptManager->runScript("GameScripts/GameManager.lua");      // Run a Lua script

    // --- END OF Buko setting up Lua/Sol -------------------------

    
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

    std::string playerTankPath = R"(Assets\game_tank\tank.gltf)";
    scene.loadPlayerModelToRegistry(playerTankPath);
    auto playerView = scene.getRegistry().view<TransformComponent, PlayerControllerComponent>();
    entt::entity playerTankEntity = entt::null;
    if (playerView.begin() != playerView.end()) {//checking if playerView is empty
        playerTankEntity = *playerView.begin();
    }
    auto& playerTankTransform = playerView.get<TransformComponent>(playerTankEntity);
    playerTankTransform.rotation.y -= 180.f;

    glm::vec3 cameraOffset = {-0.f, -5.f, -10.f};
    // for (auto entity : playerView) {
    //     auto& transform = playerView.get<TransformComponent>(entity);
    //     // transform.position = scene.getRegistry().get<TransformComponent>(cameraEntity).position + cameraOffset;
    // }

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
        auto* cameraTransform = scene.getRegistry().try_get<TransformComponent>(cameraEntity);
        playerTankTransform.position = cameraTransform->position + cameraOffset;
        glm::vec3 playerTankPos =  playerTankTransform.position;
        float terrainHeight = collision.getHeightAt(playerTankPos);
        playerTankTransform.position.y = terrainHeight + playerHeight;

        //player tank update

        
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
