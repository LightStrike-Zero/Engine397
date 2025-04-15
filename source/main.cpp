#include <Scripting/LuaManager.h>
#include <Scripting/ScriptManager.h>
#include <iostream>

#include "Factorys/RendererFactory.h"
#include "GUI/ImGui_UI.h"
#include "Interfaces/IWindow.h"
#include "ResourceManagement/Scene.h"
#include "Systems/CameraSystem.h"
#include "Systems/GridCollision.h"
#include "Terrain/TerrainFactory.h"
#include "Terrain/TerrainMenuHelper.h"
#include "Window/glfwWindow.h"
// Buko -------------------------
// Libraries for scripting
// #include <lua.hpp>
#define SOL_ALL_SAFETIES_ON 1
#include "FileHandler.h"
#include <sol/sol.hpp>
#include "FileHandler.h"

#include "Components/CollisionComponents/CollidableComponent.h"
#include "Components/PlayerControllerComponent.h"
#include "Player.h"
#include "StuffThatNeedsToBeLoadedInLua.h"
#include <fstream>
// need for exit pic
#include <glm/gtc/quaternion.hpp>

#include "Texture/TextureManager.h"
#include "imgui.h"
#include "Components/NameComponent.h"
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
    scriptManager->runScript("GameScripts/GameConfig.lua");
    std::unique_ptr<Terrain> terrain = scriptManager->createTerrainFromConfig(); // create terrain
    scene.addTerrainEntity(*terrain); // add terrain to scene

    const RawMeshData& terrainMeshData = terrain->getMeshData();

    int terrainGridRows = scriptManager->getTerrainRows();
    int terrainGridCols = scriptManager->getTerrainCols();
    float terrainScale = scriptManager->getTerrainSpacing();

    // --- END OF Buko  Lua/Sol -------------------------

    GridCollision collision(terrainGridRows, terrainGridCols, terrainScale, terrainMeshData.vertices);


    scene.setDirectionalLight(dirLight);

    /*TODO
     * this wont stay like this
     */
    auto cameraEntity = scene.getEntityManager().createEntity();
    scene.getEntityManager().addComponent<TransformComponent>(
        cameraEntity, glm::vec3(0.0f, 20.0f, 0.0f));
    // this set the player/camera start pos
    scene.getEntityManager().addComponent<CameraComponent>(cameraEntity);
    CameraSystem cameraSystem(
        static_cast<GLFWwindow*>(window->GetNativeWindow()), aspectRatio);
    Player player(
        &scene.getEntityManager(),
        static_cast<GLFWwindow*>(window->GetNativeWindow()),
        scriptManager->getFloatFromLua("playerMovementSpeed"),
        scriptManager->getFloatFromLua("playerRotationSpeed")); // added by Hugo

    // TODO should make all lua loading into one function
    std::string helpText = FileHandler::readTextFile(
        scriptManager->getHelpManualPath()); // buko: read manual text file

    static float lastFrame = 0.0f;


    //load player tank
    std::string playerTankPath = scriptManager->getStringFromLua("tankPath");
    scene.loadPlayerModelEntity(playerTankPath);
    auto playerView = scene.getEntityManager().view<TransformComponent, PlayerControllerComponent>();
    //align tank with camera orientation
    auto playerEntity = *playerView.begin();
    auto& playerTankTransform = playerView.get<TransformComponent>(playerEntity);
    playerTankTransform.rotation.y -= 180.f;
    playerTankTransform.position = scriptManager->getVec3FromLua("playerStartPos");
    playerTankTransform.position.y = collision.getHeightAt(playerTankTransform.position);
    auto& cameraTransform = scene.getEntityManager().get<TransformComponent>(cameraEntity);
    glm::vec3 cameraOffset = scriptManager->getVec3FromLua("cameraOffset");
    cameraTransform.position = playerTankTransform.position +cameraOffset;



    //randomising all the static objects' positions
    auto staticObjectsView = scene.getEntityManager().view<TransformComponent,BoxColliderComponent>(exclude<PlayerControllerComponent>);
    for (auto entity : staticObjectsView) {
        auto& staticObjectTransform = staticObjectsView.get<TransformComponent>(entity);
        float a = staticObjectTransform.position.x = rand()%terrainGridRows-terrainGridRows/2;
        float b = staticObjectTransform.position.z = rand()%terrainGridCols-terrainGridCols/2;
        staticObjectTransform.position.y = collision.getHeightAt({a, 0.f,b});
    }

    // TODO this needs to be LUA
    std::array<std::string, 6> faces = {
        "Assets/skybox/right.jpg", "Assets/skybox/left.jpg",
        "Assets/skybox/top.jpg", "Assets/skybox/bottom.jpg",
        "Assets/skybox/front.jpg", "Assets/skybox/back.jpg"
    };
    scene.createSkyBox(faces);

    //TODO delete, was test for composite texture
    // uint32_t compTexture = TextureManager::getInstance().createCompositeTexture(
    //     { "Assets/Terrain/Textures/Terrain003_2K.png", "Assets/Terrain/Textures/Mountain_01.png", "Assets/Terrain/Textures/Mountain_02.png", "Assets/Terrain/Textures/Mountain_03.png" },
    //     { 0.2f, 0.5f}
    // );

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

            // TODO remove flags showExitScreen, showHelpScreen
            cameraSystem.update(scene.getEntityManager(), deltaTime, showExitScreen,
                                showHelpScreen);
            auto [viewMatrix, projectionMatrix, viewPos] =
                cameraSystem.getActiveCameraMatrices(scene.getEntityManager());
            player.update(deltaTime,scriptManager);

            for (auto entity : playerView)
            {
                auto& playerTankTransform = playerView.get<TransformComponent>(entity);
                glm::vec3 playerTankPos = playerTankTransform.position;

                float terrainHeight = collision.getHeightAt(playerTankPos);
                float targetHeight = terrainHeight + scriptManager->getFloatFromLua("playerHeightOffset");
                float t = deltaTime * lerpSpeed; // Small factor for smooth interpolation.
                playerTankTransform.position.y = glm::mix(playerTankTransform.position.y, targetHeight, t);
                
                glm::vec3 terrainNormal = collision.getNormalAt(playerTankPos);

                glm::vec3 forwardDir = glm::normalize(glm::vec3(
                    sin(glm::radians(playerTankTransform.rotation.y)),
                    0.0f,
                    cos(glm::radians(playerTankTransform.rotation.y))
                ));

                glm::vec3 tankUp = glm::vec3(0.0f, 1.0f, 0.0f);
                glm::vec3 uphillDir = terrainNormal - glm::dot(terrainNormal, tankUp) * tankUp;
                if (glm::length(uphillDir) > 0.0001f)
                    uphillDir = glm::normalize(uphillDir);
                else
                    uphillDir = glm::vec3(0.0f); // Terrain is level.

                float dotVal = glm::dot(forwardDir, uphillDir);
                float sign = (dotVal >= 0.0f) ? -1.0f : 1.0f;

                float sampleDistance = 1.0f; // Adjust based on tank size/responsiveness.
                glm::vec3 samplePoint = playerTankPos + forwardDir * sampleDistance;
                float sampleTerrainHeight = collision.getHeightAt(samplePoint);

                float deltaHeight = fabs(sampleTerrainHeight - terrainHeight);
                float absDesiredPitch = glm::degrees(atan2(deltaHeight, sampleDistance));

                float desiredPitch = sign * absDesiredPitch;

                playerTankTransform.rotation.x = glm::mix(playerTankTransform.rotation.x, desiredPitch, t);

                playerTankTransform.rotation.z = 0.0f;
            }
            currentRenderedFrame =
                    renderer->Render(scene, viewMatrix, projectionMatrix, viewPos);
        } else {
            if (Gui.showNamedClickableImage("Click to Exit", glm::vec2{880, 510})) {
                window->SetShouldClose(true);
            }
        }
        Gui.DisplayImage("Viewport", currentRenderedFrame,
                         glm::vec2{windowWidth, windowHeight});

        if (showHelpScreen) {
            Gui.ShowHelpManual(showHelpScreen, helpText);
        }

        Gui.EndFrame();

        window->SwapBuffers();
        window->PollEvents();
    }

    Gui.Shutdown();
    delete window;
    return 0;
}

