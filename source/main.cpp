// a commnet
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Scripting/LuaManager.h"
#include "Scene.h"

#include <chrono>

#include "OpenGL/OpenGLFrameBuffer.h"
#include "OpenGL/OpenGLRenderer.h"
#include "Scripting/LuaManager.h"

// Buko -------------------------
// Libraries for scripting
#include <lua.hpp>
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
//-------------------------------

bool setupOpenGL(GLFWwindow*& window);
bool showDecal = true;
bool V_SYNC = 1;

int main(int argc, char** argv)
{
    GLFWwindow* window;
    setupOpenGL(window);
    
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");

    // --- Buko setting up Lua/Sol -------------------------
    /*
     * Lua script handles the asset paths
     * Lua script loads the models into the scene through a method in scene
     */

    LuaManager luaManager;                  // Lua Manager instance is instantiated
    Scene scene;                            // Scene instance is instantiated

    luaManager.registerScene(scene);                           // Expose Scene to Lua
    luaManager.runScript("GameScripts/GameManager.lua"); // Run a Lua script

    // --- END OF Buko setting up Lua/Sol -------------------------

    /*
     * TODO move this into a windowing manager
     */
    int windowWidth, windowHeight;
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
    float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    
    /*
     * Singleton renderer instance
     * TODO create factory that selects openGL etc
     */
    OpenGLRenderer renderer;

    /*
     * TODO move this to an input manager (potentially the window management since it's GLFW)
     */
    double lastX = 960.0, lastY = 540.0; // Center of the screen
    bool firstMouse = true;
    // Capture mouse input
    glfwSetInputMode(window, GLFW_CURSOR, /*GLFW_CURSOR_DISABLED*/GLFW_CURSOR_NORMAL);
    
    glfwWindowHint(GLFW_SAMPLES, 4);

    while (!glfwWindowShouldClose(window))
    {
        //TODO FIX camera - moved into renderer but we need to separate movement from rendering
        // camera.setAspectRatio(aspectRatio);
        // delta time
        static float lastFrame = 0.0f;
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        {
            // Exit
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window, true);
            }
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            {
                // WASD movement
                if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                    // camera.processKeyboard("FORWARD", deltaTime);
                if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                    // camera.processKeyboard("BACKWARD", deltaTime);
                if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                    // camera.processKeyboard("LEFT", deltaTime);
                if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                    // camera.processKeyboard("RIGHT", deltaTime);

                if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
                {
                    showDecal = !showDecal;
                }
                // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                // mouse movement
                double xpos, ypos;
                glfwGetCursorPos(window, &xpos, &ypos);

                if (firstMouse)
                {
                    lastX = xpos;
                    lastY = ypos;
                    firstMouse = false;
                }

                float xOffset = xpos - lastX;
                float yOffset = lastY - ypos;
                lastX = xpos;
                lastY = ypos;

                // camera.processMouseMovement(xOffset, yOffset, true);
            }
        }
        
        renderer.Render(scene);
        
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        {
            ImGui::NewFrame();
            ImGui::DockSpaceOverViewport();
        
            // Left hand window pane
            ImGui::Begin("Hello, world!");
            ImGui::Text("Direction");
            ImGui::End();
        
            // Bottom window pane
            ImGui::Begin("Another Window");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        
        
            // Viewport that the scene is rendered in
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
            ImGui::Begin("Viewport");
            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            ImGui::Image(renderer.getFrameBuffer().getTextureColorBuffer(), viewportPanelSize, ImVec2{0, 1}, ImVec2{1, 0});
            ImGui::PopStyleVar();
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            std::cerr << "OpenGL Error: " << error << std::endl;
        }


        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();




    return 0;
}

//TODO this needs to be done elsewhere, its quite old and does multiple things
bool setupOpenGL(GLFWwindow*& window)
{
    bool OpenGLSuccess = true;

    if (!glfwInit())
    {
        std::cerr << "glfwInit(): GLFW failed to initialize" << std::endl;
        OpenGLSuccess = false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1920, 1080, "Carbon Renderer", glfwGetPrimaryMonitor(), nullptr);
    if (!window)
    {
        std::cerr << "glfwCreateWindow(): Failed to create GLFW window" << std::endl;
        glfwTerminate();
        OpenGLSuccess = false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(V_SYNC); // Enable vsync


    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr << "glewInit(): GLEW failed to initialize" << std::endl;
        OpenGLSuccess = false;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    if (glfwGetWindowAttrib(window, GLFW_OPENGL_PROFILE) != GLFW_OPENGL_CORE_PROFILE)
    {
        std::cerr << "Failed to create OpenGL core profile context!" << std::endl;
        return false;
    }

    return OpenGLSuccess;
}
