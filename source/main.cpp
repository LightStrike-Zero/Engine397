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
#include <fstream>
// need for exit pic
#include "Texture/TextureManager.h"
#include "imgui.h"
//----------------------

//--
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
    float terrainScale  = scriptManager->getTerrainSpacing();

    // --- END OF Buko setting up Lua/Sol -------------------------

    GridCollision collision(terrainGridRows, terrainGridCols, terrainScale, terrainMeshData.vertices);

    DirectionalLight dirLight(glm::vec3(-0.5f, -0.9f, -0.5f), glm::vec3(0.3f, 0.3f, 0.3f),
        glm::vec3(1.0f, 0.99f, 0.99f), glm::vec3(0.09f, 0.09f, 0.09f), 0.05f);
    
    scene.setDirectionalLight(dirLight);

    auto cameraEntity = scene.getRegistry().create();
    scene.getRegistry().emplace<TransformComponent>(cameraEntity, glm::vec3(0.0f, 20.0f, 0.0f)); // this set the player/camera start pos
    scene.getRegistry().emplace<CameraComponent>(cameraEntity);
    CameraSystem cameraSystem(static_cast<GLFWwindow*>(window->GetNativeWindow()), aspectRatio);

    // buko this is for exit pic probably needs to go elsewhere!
    GLuint exitTextureID = TextureManager::getInstance().loadTexture("Assets/images/exit_pic.png");
    // end ofbuko

    // buko -------------- read manual text file
    std::string helpText;
    {
        std::ifstream file("Assets/text_files/text_manual.txt");
        if (file)
        {
            std::stringstream buffer;
            buffer << file.rdbuf();
            helpText = buffer.str();
        }
        else
        {
            helpText = "Could not load help file!";
        }
    }
    // end of buko -------------- read manual text file

    static float lastFrame = 0.0f;

    while (!window->ShouldClose())
    {
        float currentFrame = window->GetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // camera system 
        cameraSystem.update(scene.getRegistry(), deltaTime, showExitScreen, showHelpScreen);
        auto [viewMatrix, projectionMatrix, viewPos] = cameraSystem.getActiveCameraMatrices(scene.getRegistry());

        // terrian collision
        // comment this out to disable terrain collision
        auto* playerTransform = scene.getRegistry().try_get<TransformComponent>(cameraEntity);
        glm::vec3 playerPos = playerTransform->position;
        float terrainHeight = collision.getHeightAt(playerPos);
        playerTransform->position.y = terrainHeight + playerHeight;

        Gui.BeginFrame();
        Gui.DisplayImage("Viewport", renderer->Render(scene, viewMatrix, projectionMatrix, viewPos), glm::vec2{windowWidth, windowHeight});

            // buko splash screen
            if (showExitScreen)
            {
                const ImVec2 imageSize = ImVec2(880, 510); // ⬅️ Set your PNG size (adjust as needed)

                // Get screen center
                ImVec2 screenSize = ImGui::GetIO().DisplaySize;
                ImVec2 windowPos = ImVec2((screenSize.x - imageSize.x) * 0.5f, (screenSize.y - imageSize.y) * 0.5f);

                // Transparent, no decorations
                ImGui::SetNextWindowPos(windowPos);
                ImGui::SetNextWindowSize(imageSize);
                ImGui::Begin("ExitOverlay", nullptr,
                             ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoBackground |
                             ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_NoScrollWithMouse
                );

                ImGui::Image(ImTextureID(exitTextureID), imageSize);

                if (ImGui::IsItemClicked())
                {
                    glfwSetWindowShouldClose(static_cast<GLFWwindow*>(window->GetNativeWindow()), true);
                }

                ImGui::End();
            }
            // END OF buko splash screen

            // manual-------------------
                if (showHelpScreen)
                {
                    const ImVec2 helpSize = ImVec2(600, 400);  // Set a good size
                    const ImVec2 helpPos  = ImVec2(100, 100);  // Position it somewhere comfy

                    ImGui::SetNextWindowPos(helpPos, ImGuiCond_Once);
                    ImGui::SetNextWindowSize(helpSize, ImGuiCond_Once);

                    ImGui::Begin("Help Manual", &showHelpScreen,
                                 ImGuiWindowFlags_NoCollapse |
                                 ImGuiWindowFlags_NoResize);

                    ImGui::TextWrapped("%s", helpText.c_str());

                    ImGui::End();
                }
            // manual--------------------------
        Gui.EndFrame();

        window->SwapBuffers();
        window->PollEvents();
    }

    Gui.Shutdown();
    delete window;
    return 0;
}