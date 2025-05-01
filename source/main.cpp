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

#include "ResourceManagement/AssetRegistry.h"

//----------------------

//TODO EDITOR
// Global list of dropped files (persist across frames)
static std::vector<std::string> g_DroppedFiles;
//TODO EDITOR
// GLFW drop callback: called when the user drags files onto the window
static void OnFileDrop(GLFWwindow* /*wnd*/, int count, const char** paths) {
    for (int i = 0; i < count; ++i) {
        g_DroppedFiles.emplace_back(paths[i]);
    }
}



int main(int argc, char** argv)
{
    bool showExitScreen = false; // buko
    bool showHelpScreen = false; // buko

    IWindow* window = new GLFWWindow(1920, 1080, "Game Engine SHB", false);
    int windowWidth, windowHeight;
    window->GetFramebufferSize(windowWidth, windowHeight);
    float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    window->SetInputMode(CURSOR, CURSOR_NORMAL);

    IRenderer* renderer = RendererFactory::CreateRenderer(type);

    //TODO EDITOR
    AssetRegistry::Get().Initialize("Editor/assets/", "Editor/assets/registry.json");


    ImGuiUI Gui;
    Gui.Initialise(static_cast<GLFWwindow*>(window->GetNativeWindow()));

    //TODO EDITOR
    Gui.loadNamedImage("placeholder", "Assets/editor/icon.png");
    auto native = static_cast<GLFWwindow*>(window->GetNativeWindow());
    glfwSetDropCallback(native, [](GLFWwindow*, int count, const char** paths) {
        for (int i = 0; i < count; ++i) {
            // Copies each file into Assets/ and registers it
            AssetRegistry::Get().ImportAsset(paths[i]);
        }
    });

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
    // Player player(
    //     &scene.getEntityManager(),
    //     static_cast<GLFWwindow*>(window->GetNativeWindow()),
    //     scriptManager->getFloat("playerMovementSpeed"),
    //     scriptManager->getFloat("playerRotationSpeed")); // added by Hugo




    //load player tank
    std::string playerTankPath = scriptManager->getString("tankPath");
    scene.loadPlayerModelEntity(playerTankPath);
    auto playerView = scene.getEntityManager().view<TransformComponent, PlayerControllerComponent>();
    //align tank with camera orientation
    auto playerEntity = *playerView.begin();
    auto& playerTankTransform = playerView.get<TransformComponent>(playerEntity);
    playerTankTransform.rotation.y -= 180.f;
    playerTankTransform.position = scriptManager->getVec3("playerStartPos");
    playerTankTransform.position.y = collision.getHeightAt(playerTankTransform.position);
    auto& cameraTransform = scene.getEntityManager().get<TransformComponent>(cameraEntity);
    glm::vec3 cameraOffset = scriptManager->getVec3("cameraOffset");
    cameraTransform.position = playerTankTransform.position +cameraOffset;



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
    static bool showNewArchetypePopup = false;
    Gui.loadNamedImage("Click to Exit", scriptManager->getSplashImagePath()); // buko
    unsigned int currentRenderedFrame;
    while (!window->ShouldClose())
    {
        Gui.BeginFrame();

        if (!showExitScreen)
        {
            // 1) Compute delta
            float currentFrame = window->GetTime();
            float deltaTime   = currentFrame - lastFrame;
            lastFrame         = currentFrame;

            // 2) Drive the editor‐viewport camera
            cameraSystem.update(
                scene.getEntityManager(),
                deltaTime,
                /*showExitScreen=*/ showExitScreen,
                /*showHelpScreen=*/ showHelpScreen
            );

            // 3) Grab view/proj and render the entire scene
            auto [viewMatrix, projMatrix, viewPos] =
                cameraSystem.getActiveCameraMatrices(scene.getEntityManager());

            currentRenderedFrame =
                renderer->Render(scene, viewMatrix, projMatrix, viewPos);
        }
        else
        {
            // still keep your exit‐screen logic
            if (Gui.showNamedClickableImage("Click to Exit", {880, 510}))
                window->SetShouldClose(true);
        }

        // Content browser
         Gui.BeginWindow("Content Browser", nullptr, 0);

    // + New Archetype button
    if (ImGui::Button("+ New Archetype"))
        showNewArchetypePopup = true;
    ImGui::SameLine();
    // ImGui::TextUnformatted("Content Browser");
    ImGui::Separator();

    // 1) Raw Assets section
    if (ImGui::CollapsingHeader("Raw Assets", ImGuiTreeNodeFlags_DefaultOpen))
    {
        const auto& assets = AssetRegistry::Get().GetAssets();
        if (assets.empty())
        {
            ImGui::TextUnformatted("Drag asset files here to import.");
        }
        else
        {
            const int iconSize = 64, padding = 10;
            float availW = ImGui::GetContentRegionAvail().x;
            int cols = std::max(1, int(availW / (iconSize + padding)));
            int i = 0;

            for (auto& entry : assets)
            {
                ImGui::PushID(entry.destPath.c_str());
                ImGui::BeginGroup();

                // Thumbnail-button + drag source
                if (Gui.showNamedClickableImage(entry.thumbKey, { iconSize, iconSize }))
                {
                    // optional: select/focus this asset
                }
                if (ImGui::BeginDragDropSource())
                {
                    ImGui::SetDragDropPayload(
                        "CONTENT_BROWSER_ASSET",
                        entry.destPath.c_str(),
                        entry.destPath.size() + 1
                    );
                    ImGui::Text("Asset: %s", entry.name.c_str());
                    ImGui::EndDragDropSource();
                }

                ImGui::TextWrapped("%s", entry.name.c_str());
                ImGui::EndGroup();
                ImGui::PopID();

                if (++i < (int)assets.size() && (i % cols) != 0)
                    ImGui::SameLine();
            }
        }
    }

    // 2) Archetypes section
    if (ImGui::CollapsingHeader("Archetypes", ImGuiTreeNodeFlags_DefaultOpen))
    {
        // for now just show placeholder text
        ImGui::TextUnformatted("(archetypes will go here)");
        // Later you’ll replace this with a grid similar to Raw Assets,
        // but using PrototypeRegistry::Get().GetArchetypes()
    }

    Gui.EndWindow();  // end Content Browser

    // === Create Archetype Modal ===
    if (showNewArchetypePopup)
    {
        ImGui::OpenPopup("Create Archetype");
        showNewArchetypePopup = false;
    }
    if (ImGui::BeginPopupModal("Create Archetype",
                              nullptr,
                              ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::TextUnformatted("Archetype Editor (placeholder)\n"
                               "Here you’ll choose components, assign assets, etc.");
        ImGui::Separator();
        if (ImGui::Button("Close"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }

        // Viewport window
        Gui.BeginWindow("Viewport", nullptr, 0);
        // Gui.DisplayImage("Viewport", currentRenderedFrame, { windowWidth, windowHeight });
        Gui.ImageWidget(currentRenderedFrame, { windowWidth, windowHeight });
        Gui.EndWindow();

        // Detail panel
        Gui.BeginWindow("Details", nullptr, 0);
        ImGui::Text("(entity inspector)");
        Gui.EndWindow();

        // 5) Help window if requested
        if (showHelpScreen)
            Gui.ShowHelpManual(showHelpScreen, helpText);

        // 6) Finish ImGui & swap
        Gui.EndFrame();
        window->SwapBuffers();
        window->PollEvents();
    }

    Gui.Shutdown();
    delete window;
    return 0;
}

