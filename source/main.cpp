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
#include <glm/gtc/quaternion.hpp>

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

    ScriptManager* scriptManager = new LuaManager(); // Lua Manager instance is instantiated derived from base class
    scriptManager->registerScene(scene); // Expose Scene to Lua
    std::unique_ptr<Terrain> terrain = scriptManager->createTerrainFromConfig(); // create terrain
    scene.addTerrainEntity(*terrain); // add terrain to scene

    const RawMeshData& terrainMeshData = terrain->getMeshData();

    int terrainGridRows = scriptManager->getTerrainRows();
    int terrainGridCols = scriptManager->getTerrainCols();
    float terrainScale = scriptManager->getTerrainSpacing();

    // --- END OF Buko  Lua/Sol -------------------------

    GridCollision collision(terrainGridRows, terrainGridCols, terrainScale, terrainMeshData.vertices);


    scene.setDirectionalLight(dirLight);
    // scene.loadModelToRegistry(backPackPath);

    /*TODO
     * this wont stay like this
     */
    auto cameraEntity = scene.getEntityManager().createEntity();
    scene.getEntityManager().addComponent<TransformComponent>(cameraEntity, glm::vec3(0.0f, 20.0f, 0.0f));
    // this set the player/camera start pos
    scene.getEntityManager().addComponent<CameraComponent>(cameraEntity);
    CameraSystem cameraSystem(static_cast<GLFWwindow*>(window->GetNativeWindow()), aspectRatio);
    Player player(&scene.getEntityManager(), static_cast<GLFWwindow*>(window->GetNativeWindow())); //added by Hugo

    //TODO should make all lua loading into one function
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
    auto& cameraTransform = scene.getEntityManager().get<TransformComponent>(cameraEntity);
    glm::vec3 cameraOffset = {-0.0f, -30.0f, -50.0f}; //so camera isn't sitting inside tank
    //align tank with camera orientation
    for (auto entity : playerView)
    {
        auto& playerTankTransform = playerView.get<TransformComponent>(entity);
        playerTankTransform.rotation.y -= 180.f;
        playerTankTransform.position = cameraTransform.position + cameraOffset;
    }

    std::string tankPath = R"(Assets\game_tank\tank.gltf)";
    std::string jeepPath = R"(Assets\game_jeep\jeep.gltf)";
    std::string rock1Path = R"(Assets\game_rock1\rock1.gltf)";
    std::string rock2Path = R"(Assets\game_rock2\rock2.gltf)";
    std::string tree1Path = R"(Assets\game_tree1_dead_small\tree1.gltf)";
    std::string tree2Path = R"(Assets\game_tree2_dead_big\tree2.gltf)";
    std::string tree3Path = R"(Assets\game_tree3_pine_narrow\tree3.gltf)";
    std::string tree4Path = R"(Assets\game_tree4_pine2_wide\tree4.gltf)";
    for (int i = 0; i < 3; ++i)
    {
        scene.loadCollidableBoxEntity(tree1Path);
        scene.loadCollidableBoxEntity(jeepPath);
        scene.loadCollidableBoxEntity(rock1Path);
        scene.loadModelEntity(tree1Path);
        scene.loadCollidableCapsuleEntity(tree1Path);
        scene.loadCollidableBoxEntity(rock2Path);
        scene.loadCollidableBoxEntity(jeepPath);
    }
    auto staticObjectsView = scene.getEntityManager().view<TransformComponent, BoxColliderComponent>(
        exclude<PlayerControllerComponent>);

    for (auto entity : staticObjectsView)
    {
        auto& staticObjectTransform = staticObjectsView.get<TransformComponent>(entity);
        float a = staticObjectTransform.position.x = rand() % 100 - 50;
        float b = staticObjectTransform.position.z = rand() % 100 - 50;
        staticObjectTransform.position.y = collision.getHeightAt({a, 0.f, b});
    }

    //TODO this needs to be LUA
    std::array<std::string, 6> faces = {
        "Assets/skybox/right.jpg", "Assets/skybox/left.jpg", "Assets/skybox/top.jpg", "Assets/skybox/bottom.jpg",
        "Assets/skybox/front.jpg", "Assets/skybox/back.jpg"
    };
    scene.createSkyBox(faces);

    float lerpSpeed = 10.0f;

    Gui.loadNamedImage("Click to Exit", scriptManager->getSplashImagePath()); // buko
    unsigned int currentRenderedFrame;
    while (!window->ShouldClose())
    {
        Gui.BeginFrame();
        if (!showExitScreen)
        {
            float currentFrame = window->GetTime();
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // camera system
            //TODO remove flags showExitScreen, showHelpScreen
            cameraSystem.update(scene.getEntityManager(), deltaTime, showExitScreen, showHelpScreen);
            auto [viewMatrix, projectionMatrix, viewPos] = cameraSystem.getActiveCameraMatrices(
                scene.getEntityManager());
            player.update(deltaTime);

            // Terrain collision and pitch adjustment
            for (auto entity : playerView)
            {
                auto& playerTankTransform = playerView.get<TransformComponent>(entity);
                glm::vec3 playerTankPos = playerTankTransform.position;


                // 1. Update vertical position using height interpolation.
                float terrainHeight = collision.getHeightAt(playerTankPos);
                float targetHeight = terrainHeight + playerHeight;
                float t = deltaTime * lerpSpeed; // Small factor for smooth interpolation.
                playerTankTransform.position.y = glm::mix(playerTankTransform.position.y, targetHeight, t);
                cameraTransform.position = playerTankTransform.position - cameraOffset;

                // 2. Compute the forward direction using the tank's current yaw.
                //    (Assuming that when rotation.y is 0 the tank faces along +Z.)
                glm::vec3 forwardDir = glm::normalize(glm::vec3(
                    sin(glm::radians(playerTankTransform.rotation.y)),
                    0.0f,
                    cos(glm::radians(playerTankTransform.rotation.y))
                ));
                
                
                // 5. Sample the terrain height in the direction the tank is moving.
                float sampleDistance = 1.0f; // Adjust based on tank size and desired responsiveness.
                glm::vec3 samplePoint = playerTankPos + sampleDir * sampleDistance;
                float sampleTerrainHeight = collision.getHeightAt(samplePoint);

                // 6. Compute the desired pitch from the height difference.
                //    (Using atan2 gives an angle with the correct sign based on the delta height.)
                float deltaHeight = sampleTerrainHeight - terrainHeight;
                float desiredPitch = glm::degrees(atan2(deltaHeight, sampleDistance));

                // 7. If the tank is reversing, invert the pitch to match the reversed motion.
                if (isReversing)
                {
                    desiredPitch = -desiredPitch;
                }

                // 8. Smoothly interpolate (lerp) the current pitch (rotation.x) toward the desired pitch.
                float newPitch = glm::mix(playerTankTransform.rotation.x, desiredPitch, t);
                playerTankTransform.rotation.x = newPitch;

                // 9. Preserve the current yaw and lock roll to zero.
                playerTankTransform.rotation.z = 0.0f;
            }
            currentRenderedFrame = renderer->Render(scene, viewMatrix, projectionMatrix, viewPos);
        }
        else
        {
            if (Gui.showNamedClickableImage("Click to Exit", glm::vec2{880, 510}))
            {
                window->SetShouldClose(true);
            }
        }
        Gui.DisplayImage("Viewport", currentRenderedFrame, glm::vec2{windowWidth, windowHeight});


        if (showHelpScreen)
        {
            Gui.ShowHelpManual(showHelpScreen, helpText);
        }

        Gui.EndFrame();

        window->SwapBuffers();
        window->PollEvents();
        // window->pollInputEvents();
    }

    Gui.Shutdown();
    delete window;
    return 0;
}
