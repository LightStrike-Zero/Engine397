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
#include "Components/CollidableComponent.h"
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

    //load player tank
    std::string playerTankPath = R"(Assets\game_tank\tank.gltf)";
    scene.loadPlayerModelToRegistry(playerTankPath);
    auto playerView = scene.getRegistry().view<TransformComponent, PlayerControllerComponent>();
    //align tank with camera orientation
    for (auto entity : playerView) {
        auto& playerTankTransform = playerView.get<TransformComponent>(entity);
        playerTankTransform.rotation.y -= 180.f;
        std::cout << "player pos:" << playerTankTransform.position.x << ", " << playerTankTransform.position.y << ", "
        << playerTankTransform.position.z << std::endl;
    }
    glm::vec3 cameraOffset = {-0.f, -5.f, -10.f}; //so camera isn't sitting inside tank

    std::string tankPath = R"(Assets\game_tank\tank.gltf)";
    std::string jeepPath = R"(Assets\game_jeep\jeep.gltf)";
    std::string jeepTestPath = R"(Assets\game_jeep_gltf\jeep2.gltf)";
    std::string jeepTestPath2 = R"(Assets\game_jeep_glb\jeep2.glb)";
    std::string rock1Path = R"(Assets\game_rock1\rock1.gltf)";
    std::string rock2Path = R"(Assets\game_rock2\rock2.gltf)";
    std::string tree1Path = R"(Assets\game_tree1_dead_small\trees_dead_small.gltf)";
    std::string tree2Path = R"(Assets\game_tree2_dead_big\trees_dead_big.gltf)";
    std::string tree3Path = R"(Assets\game_tree3_pine_narrow\trees_narrow.gltf)";
    std::string tree4Path = R"(Assets\game_tree4_pine2_wide\trees_wide.gltf)";
    for (int i = 0; i < 3; ++i) {
        scene.loadCollidableModelToRegistry(tree1Path);
        // scene.loadCollidableModelToRegistry(rock2Path);
        scene.loadCollidableModelToRegistry(jeepTestPath2);
    }
    auto staticObjectsView = scene.getRegistry().view<TransformComponent,CollidableComponent>();

    for (auto entity : staticObjectsView) {
        auto& staticObjectTransform = staticObjectsView.get<TransformComponent>(entity);
        float a = staticObjectTransform.position.x = rand()%100-50;
        float b = staticObjectTransform.position.z = rand()%100-50;
        staticObjectTransform.position.y = collision.getHeightAt({a, 0.f,b});
    }




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
        for (auto entity : playerView) { //tank updates y value according to terrain, adjust camera accordingly
            auto& playerTankTransform = playerView.get<TransformComponent>(entity);
            playerTankTransform.position = cameraTransform->position + cameraOffset;
            glm::vec3 playerTankPos =  playerTankTransform.position;
            float terrainHeight = collision.getHeightAt(playerTankPos);
            playerTankTransform.position.y = terrainHeight + playerHeight;
            cameraTransform->position = playerTankTransform.position - cameraOffset;
        }



        
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
