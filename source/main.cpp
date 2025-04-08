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
//----------------------

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
    
    Scene scene;
    
    // --- Buko setting up Lua/Sol -------------------------
    /*
     * ScriptManager dynamic allocation using a pointer: flexibility, type of object can be changed at runtime
     * Lua script handles the asset paths
     * Lua script loads the models into the scene through a method in scene
     */

    ScriptManager* scriptManager = new LuaManager();      // Lua Manager instance is instantiated derived from base class

    scriptManager->registerScene(scene);                                      // Expose Scene to Lua
    scriptManager->runScript("GameScripts/GameManager.lua");            // Run a Lua script
    scriptManager->runScript("GameScripts/TerrainConfig_fractal.lua");  // Get terrain parameters

    // Read terrain config from Lua
    sol::table terrainConfig = static_cast<LuaManager*>(scriptManager)->getTerrainConfig();

    // Print parameter values from lua script
    std::string paramType = terrainConfig["type"].get<std::string>();
    std::cout << "[DEBUG LUA] Terrain type: " << paramType << "\n";

    int paramRows = terrainConfig["rows"].get<int>();
    std::cout << "[DEBUG LUA] Terrain rows: " << paramRows << "\n";

    int paramCols = terrainConfig["cols"].get<int>();
    std::cout << "[DEBUG LUA] Terrain columns: " << paramCols << "\n";

    float paramSpacing = terrainConfig["spacing"].get<float>();
    std::cout << "[DEBUG LUA] Terrain spacing: " << paramSpacing << "\n";

    int paramIterations = terrainConfig["parameters"]["iterations"].get<int>();
    std::cout << "[DEBUG LUA] Terrain iterations: " << paramIterations << "\n";

    float paramInitialDisplacement = terrainConfig["parameters"]["initialDisplacement"].get<float>();
    std::cout << "[DEBUG LUA] Terrain initialDisplacement: " << paramInitialDisplacement << "\n";

    float paramDisplacementDecay = terrainConfig["parameters"]["displacementDecay"].get<float>();
    std::cout << "[DEBUG LUA] Terrain displacementDecay: " << paramDisplacementDecay << "\n";

    float paramHeightScale = terrainConfig["parameters"]["heightScale"].get<float>();
    std::cout << "[DEBUG LUA] Terrain heightScale: " << paramHeightScale << "\n";

    int paramSeed = terrainConfig["parameters"]["seed"].get<int>();
    std::cout << "[DEBUG LUA] Terrain seed: " << paramSeed << "\n";

    float paramSmoothness = terrainConfig["parameters"]["smoothness"].get<float>();
    std::cout << "[DEBUG LUA] Terrain smoothness: " << paramSmoothness << "\n";

    int paramSmoothingPasses = terrainConfig["parameters"]["smoothingPasses"].get<int>();
    std::cout << "[DEBUG LUA] Terrain smoothingPasses: " << paramSmoothingPasses << "\n";

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
