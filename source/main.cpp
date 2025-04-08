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

//--
int main(int argc, char** argv)
{
    float playerHeight = 1.0f;

    IWindow* window = new GLFWWindow(1920, 1080, "Game Engine SHB", true);
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
    //ScriptManager dynamic allocation using a pointer: flexibility, type of object can be changed at runtime

    ScriptManager* scriptManager = new LuaManager();      // Lua Manager instance is instantiated derived from base class

    scriptManager->registerScene(scene);                                         // Expose Scene to Lua
    std::unique_ptr<Terrain> terrain = scriptManager->createTerrainFromConfig();    // create terrain
    scene.addTerrainToScene(*terrain);                                       // add terrain to scene

    const RawMeshData& terrainMeshData = terrain->getMeshData();

    int terrainGridRows = scriptManager->getTerrainRows();
    int terrainGridCols = scriptManager->getTerrainCols();
    float terrainScale   = scriptManager->getTerrainSpacing();

    // --- END OF Buko setting up Lua/Sol -------------------------

    GridCollision collision(terrainGridRows, terrainGridCols, terrainScale, terrainMeshData.vertices);

    DirectionalLight dirLight(glm::vec3(-0.5f, -0.9f, -0.5f), glm::vec3(0.3f, 0.3f, 0.3f),
        glm::vec3(1.0f, 0.99f, 0.99f), glm::vec3(0.09f, 0.09f, 0.09f), 0.05f);
    
    scene.setDirectionalLight(dirLight);

    auto cameraEntity = scene.getRegistry().create();
    scene.getRegistry().emplace<TransformComponent>(cameraEntity, glm::vec3(0.0f, 20.0f, 0.0f)); // this set the player/camera start pos
    scene.getRegistry().emplace<CameraComponent>(cameraEntity);
    CameraSystem cameraSystem(static_cast<GLFWwindow*>(window->GetNativeWindow()), aspectRatio);

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