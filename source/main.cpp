#include <Scripting/LuaManager.h>
#include <Scripting/ScriptManager.h>
#include "Factorys/RendererFactory.h"
#include "GUI/ImGui_UI.h"
#include "Interfaces/IWindow.h"
#include "ResourceManagement/Scene.h"
#include "Systems/CameraSystem.h"
#include "Systems/GridCollision.h"
#include "Terrain/TerrainFactory.h"
#include "Window/glfwWindow.h"
#define SOL_ALL_SAFETIES_ON 1
#include "FileHandler.h"
#include <sol/sol.hpp>
#include "Components/PlayerControllerComponent.h"
#include "Player.h"
#include "StuffThatNeedsToBeLoadedInLua.h"
#include <fstream>
#include <glm/gtc/quaternion.hpp>

//----------------------

int main(int argc, char** argv)
{
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
    
    ScriptManager* scriptManager = new LuaManager(); // Lua Manager instance is instantiated derived from base class
    scriptManager->registerScene(scene); // Expose Scene to Lua
    scriptManager->runScript("GameScripts/GameConfig.lua");
    std::unique_ptr<Terrain> terrain = scriptManager->createTerrainFromConfig(); // create terrain
    scene.addTerrainEntity(*terrain); // add terrain to scene

    const RawMeshData& terrainMeshData = terrain->getMeshData();

    int terrainGridRows = scriptManager->getTerrainRows();
    int terrainGridCols = scriptManager->getTerrainCols();
    float terrainScale = scriptManager->getTerrainSpacing();
    
    GridCollision collision(terrainGridRows, terrainGridCols, terrainScale, terrainMeshData.vertices);


    scene.setDirectionalLight(dirLight);

    
    // CAMERA SET UP
    auto cameraEntity = scene.getEntityManager().createEntity();
    scene.getEntityManager().addComponent<TransformComponent>(cameraEntity, glm::vec3(0.0f, 20.0f, 0.0f));
    scene.getEntityManager().addComponent<CameraComponent>(cameraEntity);
    CameraSystem cameraSystem(static_cast<GLFWwindow*>(window->GetNativeWindow()), aspectRatio);

    // PLAYER SET UP
    Player player(&scene.getEntityManager(),static_cast<GLFWwindow*>(window->GetNativeWindow()),scriptManager->getFloatFromLua("playerMovementSpeed"),scriptManager->getFloatFromLua("playerRotationSpeed")); // added by Hugo

    std::string playerTankPath = scriptManager->getStringFromLua("tankPath");
    scene.loadPlayerModelEntity(playerTankPath);
    auto playerView = scene.getEntityManager().view<TransformComponent, PlayerControllerComponent>();
    
    
    // ALIGN TANK
    for (auto entity : playerView) {
        auto& playerTankTransform = playerView.get<TransformComponent>(entity);
        playerTankTransform.rotation.y -= 180.f;
        playerTankTransform.position = scriptManager->getVec3FromLua("playerStartPos");
    }

    // SPAWN STATIC OBJECTS
    auto staticObjectsView = scene.getEntityManager().view<TransformComponent,BoxColliderComponent>(exclude<PlayerControllerComponent>);
    for (auto entity : staticObjectsView) {
        auto& staticObjectTransform = staticObjectsView.get<TransformComponent>(entity);
        float a = staticObjectTransform.position.x = rand()%terrainGridRows-terrainGridRows/2;
        float b = staticObjectTransform.position.z = rand()%terrainGridCols-terrainGridCols/2;
        staticObjectTransform.position.y = collision.getHeightAt({a, 0.f,b});
    }

    
    std::array<std::string, 6> skyboxFaces = scriptManager->getSkyboxFaces();
    scene.createSkyBox(skyboxFaces);
    
    std::string helpText = FileHandler::readTextFile(scriptManager->getHelpManualPath());
    static float lastFrame = 0.0f;
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
                float t = deltaTime * lerpSpeed; 
                playerTankTransform.position.y = glm::mix(playerTankTransform.position.y, targetHeight, t);

                //----------------- new camera rotation logic -----------------
                
                glm::vec3 baseOffset = scriptManager->getVec3FromLua("baseOffset");

                float tankYaw = playerTankTransform.rotation.y;


                // Rotate offset around Y axis to follow behind tank
                glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(tankYaw), glm::vec3(0, 1, 0));
                glm::vec3 rotatedOffset = glm::vec3(rotationMatrix * glm::vec4(baseOffset, 0.0f));

                // New camera position
                auto &cameraTransform = scene.getEntityManager().get<TransformComponent>(cameraEntity);
                cameraTransform.position = playerTankTransform.position + rotatedOffset;
                //
                // // Let camera face the tank
                // auto &cameraComponent = scene.getEntityManager().get<CameraComponent>(cameraEntity);
                // glm::vec3 lookDir = glm::normalize(playerTankTransform.position - cameraTransform.position);
                // cameraComponent.yaw = glm::degrees(atan2(lookDir.z, lookDir.x));
                // cameraComponent.pitch = glm::degrees(asin(lookDir.y));
                // //
                // // // Update front vector
                // cameraComponent.front = glm::normalize(glm::vec3{
                //     cos(glm::radians(cameraComponent.yaw)) * cos(glm::radians(cameraComponent.pitch)),
                //     sin(glm::radians(cameraComponent.pitch)),
                //     sin(glm::radians(cameraComponent.yaw)) * cos(glm::radians(cameraComponent.pitch))
                // });


                //----------------- THE END ------------------

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
        Gui.DisplayImage("Viewport", currentRenderedFrame, glm::vec2{windowWidth, windowHeight});

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

