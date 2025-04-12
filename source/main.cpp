#include <iostream>
#include <Scripting/LuaManager.h>
#include <Scripting/ScriptManager.h>

#include "GUI/ImGui_UI.h"
#include "ResourceManagement/Scene.h"
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
#include "FileHandler.h"

#include "Player.h"
#include "Components/PlayerControllerComponent.h"
#include "StuffThatNeedsToBeLoadedInLua.h"
#include "Components/CollisionComponents/CollidableComponent.h"
#include <fstream>
// need for exit pic
#include "Texture/TextureManager.h"
#include "imgui.h"
//----------------------

int main(int argc, char** argv)
{
    float playerHeight = 1.0f;
    bool showExitScreen = false; // buko
    bool showHelpScreen = false; // buko

    IWindow* window = new GLFWWindow(1920, 1080, "Game Engine SHB", true);
    int windowWidth, windowHeight;
    window->GetFramebufferSize(windowWidth, windowHeight);
    float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    window->SetInputMode(CURSOR, CURSOR_NORMAL);

    IRenderer* renderer = RendererFactory::CreateRenderer(type);

    ImGuiUI Gui;
    Gui.Initialise(static_cast<GLFWwindow*>(window->GetNativeWindow()));

    Scene scene;
    
    // --- Buko setting up Lua/Sol -------------------------
    //ScriptManager dynamic allocation using a pointer: flexibility, type of object can be changed at runtime

    ScriptManager* scriptManager = new LuaManager();      // Lua Manager instance is instantiated derived from base class
    scriptManager->registerScene(scene);                                         // Expose Scene to Lua
    std::unique_ptr<Terrain> terrain = scriptManager->createTerrainFromConfig();    // create terrain
    scene.addTerrainEntity(*terrain);                                       // add terrain to scene

    const RawMeshData& terrainMeshData = terrain->getMeshData();

    int terrainGridRows = scriptManager->getTerrainRows();
    int terrainGridCols = scriptManager->getTerrainCols();
    float terrainScale  = scriptManager->getTerrainSpacing();

    // --- END OF Buko  Lua/Sol -------------------------

    GridCollision collision(terrainGridRows, terrainGridCols, terrainScale, terrainMeshData.vertices);

    DirectionalLight dirLight(glm::vec3(-0.5f, -0.9f, -0.5f), glm::vec3(0.3f, 0.3f, 0.3f),
        glm::vec3(1.0f, 0.99f, 0.99f), glm::vec3(0.09f, 0.09f, 0.09f), 0.05f);
    
    scene.setDirectionalLight(dirLight);
    // scene.loadModelToRegistry(backPackPath);

    /*TODO
     * this wont stay like this
     */
    auto cameraEntity = scene.getEntityManager().createEntity();
    scene.getEntityManager().addComponent<TransformComponent>(cameraEntity, glm::vec3(0.0f, 20.0f, 0.0f)); // this set the player/camera start pos
    scene.getEntityManager().addComponent<CameraComponent>(cameraEntity);
    CameraSystem cameraSystem(static_cast<GLFWwindow*>(window->GetNativeWindow()), aspectRatio);
    Player player(&scene.getEntityManager(),static_cast<GLFWwindow*>(window->GetNativeWindow()));//added by Hugo

    std::string helpText = FileHandler::readTextFile(scriptManager->getHelpManualPath()); // buko: read manual text file

    static float lastFrame = 0.0f;

    // entt::entity playerTankEntity = entt::null;
    // if (playerView.begin() != playerView.end()) {//checking if playerView is empty
        // playerTankEntity = *playerView.begin();
    // }
    //load player tank
    scene.loadPlayerModelEntity(playerTankPath); //this gets playerTankPath from Lua, the right way
    // std::string playerTankPath = R"(Assets\game_tank\tank.gltf)";
    // scene.loadPlayerModelEntity(playerTankPath); //wrong way, but improvising for now
    auto playerView = scene.getEntityManager().view<TransformComponent, PlayerControllerComponent>();
    auto &cameraTransform = scene.getEntityManager().get<TransformComponent>(cameraEntity);
    glm::vec3 cameraOffset = {-0.f, -5.f, -10.f}; //so camera isn't sitting inside tank
    //align tank with camera orientation
    for (auto entity : playerView) {
        auto& playerTankTransform = playerView.get<TransformComponent>(entity);
        playerTankTransform.rotation.y -= 180.f;
        playerTankTransform.position = cameraTransform.position+cameraOffset;

    }

    std::string tankPath = R"(Assets\game_tank\tank.gltf)";
    std::string jeepPath = R"(Assets\game_jeep\jeep.gltf)";
    std::string rock1Path = R"(Assets\game_rock1\rock1.gltf)";
    std::string rock2Path = R"(Assets\game_rock2\rock2.gltf)";
    std::string tree1Path = R"(Assets\game_tree1_dead_small\trees_dead_small.gltf)";
    std::string tree2Path = R"(Assets\game_tree2_dead_big\trees_dead_big.gltf)";
    std::string tree3Path = R"(Assets\game_tree3_pine_narrow\trees_narrow.gltf)";
    std::string tree4Path = R"(Assets\game_tree4_pine2_wide\trees_wide.gltf)";
    for (int i = 0; i < 3; ++i) {
        scene.loadCollidableBoxEntity(tree1Path);
        // scene.loadCollidableBoxEntity(jeepPath);
        // scene.loadCollidableBoxEntity(rock1Path);
        // scene.loadModelEntity(tree1Path);
        // scene.loadCollidableCapsuleEntity(tree1Path);
        // scene.loadCollidableEntity(rock2Path);
        // scene.loadCollidableBoxEntity(jeepPath);
    }
    auto staticObjectsView = scene.getEntityManager().view<TransformComponent,BoxColliderComponent>(exclude<PlayerControllerComponent>);

    for (auto entity : staticObjectsView) {
        auto& staticObjectTransform = staticObjectsView.get<TransformComponent>(entity);
        float a = staticObjectTransform.position.x = rand()%100-50;
        float b = staticObjectTransform.position.z = rand()%100-50;
        staticObjectTransform.position.y = collision.getHeightAt({a, 0.f,b});
    }

    Gui.loadNamedImage("Click to Exit", scriptManager->getSplashImagePath()); // buko
    unsigned int currentRenderedFrame;
    while (!window->ShouldClose())
    {
        Gui.BeginFrame();
        if (!showExitScreen){
            float currentFrame = window->GetTime();
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // camera system 
            cameraSystem.update(scene.getEntityManager(), deltaTime, showExitScreen, showHelpScreen);
            auto [viewMatrix, projectionMatrix, viewPos] = cameraSystem.getActiveCameraMatrices(scene.getEntityManager());
        // camera system 
        cameraSystem.update(scene.getEntityManager(), deltaTime);
        auto [viewMatrix, projectionMatrix, viewPos] = cameraSystem.getActiveCameraMatrices(scene.getEntityManager());
        player.update(deltaTime);

            // terrian collision
            // comment this out to disable terrain collision
            auto &cameraTransform = scene.getEntityManager().get<TransformComponent>(cameraEntity);
            for (auto entity : playerView) {
                auto& playerTankTransform = playerView.get<TransformComponent>(entity);
                playerTankTransform.position = cameraTransform.position + cameraOffset;
                glm::vec3 playerTankPos =  playerTankTransform.position;
                float terrainHeight = collision.getHeightAt(playerTankPos);
                playerTankTransform.position.y = terrainHeight + playerHeight;
                cameraTransform.position = playerTankTransform.position - cameraOffset;
                //std::cout << "Player Tank Position: " << playerTankTransform.position.x << ", " << playerTankTransform.position.y << ", " << playerTankTransform.position.z << std::endl;
            }
            currentRenderedFrame = renderer->Render(scene, viewMatrix, projectionMatrix, viewPos);
        // terrian collision
        // comment this out to disable terrain collision
        // auto &cameraTransform = scene.getEntityManager().get<TransformComponent>(cameraEntity);
        for (auto entity : playerView) {
            auto& playerTankTransform = playerView.get<TransformComponent>(entity);
            glm::vec3 playerTankPos =  playerTankTransform.position;
            float terrainHeight = collision.getHeightAt(playerTankPos);
            playerTankTransform.position.y = terrainHeight + playerHeight;
            cameraTransform.position = playerTankTransform.position - cameraOffset;
            // std::cout << "Player Tank Position: " << playerTankTransform.position.x << ", " << playerTankTransform.position.y << ", " << playerTankTransform.position.z << std::endl;
        }
        else
        {
            
            if (Gui.showNamedClickableImage("Click to Exit", glm::vec2{880, 510}))
            {
                window->SetShouldClose(true);
            }
        }
        Gui.DisplayImage("Viewport", currentRenderedFrame, glm::vec2{windowWidth, windowHeight});



        // Buko help manual system
        if (showHelpScreen)
        {
            Gui.ShowHelpManual(showHelpScreen, helpText);
        }
        // buko splash screen
        // if (showExitScreen)
        // {
        // }
        // manual--------------------------
        Gui.EndFrame();

        window->SwapBuffers();
        window->PollEvents();
        window->pollInputEvents();//custom, not part of glfw

    }

    Gui.Shutdown();
    delete window;
    return 0;
}
