#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Camera.h>
#include <iostream>
#include <Lights/Light.h>
#include <Renderer.h>
#include <Shader.h>


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Framebuffer.h"
#include "Scene.h"
#include "ShaderManager.h"

#include <chrono>

#include "Lights/DirectionalLight.h"
#include "Lights/PointLight.h"

bool setupOpenGL(GLFWwindow*& window);
bool showDecal = true;
bool V_SYNC = 0;

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

    io.FontDefault = io.Fonts->AddFontFromFileTTF(R"(Z:\Code Projects\CarbonEngine\Assets\font\roboto\static\Roboto-Regular.ttf)", 18.0f);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

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

    glEnable(GL_DEPTH_TEST);    //TODO why are we doing this here?

    // Load shaders
    // Shader lightingShader("new_vertex.glsl", "new_fragment.glsl");
    // Shader shadowShader("shadow_vertex.glsl","shadow_fragment.glsl");
    // Shader framebufferShader("framebuffer.vert","framebuffer.frag");

    ShaderManager shaderManager;

    shaderManager.loadShader("lightingShader", "new_vertex.glsl", "new_fragment.glsl");
    shaderManager.loadShader("shadowShader", "shadow_vertex.glsl", "shadow_fragment.glsl");
    shaderManager.loadShader("framebufferShader", "framebuffer.vert", "framebuffer.frag");

    auto lightingShader = shaderManager.getShader("lightingShader");
    // auto shadowShader = shaderManager.getShader("shadowShader");
    auto framebufferShader = shaderManager.getShader("framebufferShader");

    Scene scene;

    std::string backPackPath = (R"(Assets\survival_guitar_backpack_scaled\scene.gltf)");
    std::string sponzaPath = (R"(Assets\main1_sponza\NewSponza_Main_glTF_003.gltf)");
    // comment out the blow to disable loading
    scene.loadModelToRegistry(backPackPath);
    scene.loadModelToRegistry(sponzaPath);


    int windowWidth, windowHeight;
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
    float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

    // Initialize the camera
    Camera camera(
        glm::vec3(0.0f, 0.0f, 6.0f), // Position
        glm::vec3(1.0f, 1.0f, 0.0f), // Target
        glm::vec3(0.0f, 1.0f, 0.0f), // Up
        2.5f, // Speed
        0.5f, // Collider radius
        45.0f, // Field of view
        aspectRatio, // Aspect ratio
        0.1f, // Near plane
        100.0f // Far plane
    );

    Renderer renderer;

    double lastX = 960.0, lastY = 540.0; // Center of the screen
    bool firstMouse = true;

    // Capture mouse input
    glfwSetInputMode(window, GLFW_CURSOR, /*GLFW_CURSOR_DISABLED*/GLFW_CURSOR_NORMAL);

    DirectionalLight dirLight(glm::vec3(0.5f, -1.0f, -0.5f), glm::vec3(0.09f, 0.09f, 0.1f),
        glm::vec3(0.79f, 0.79f, 0.85f), glm::vec3(0.39f, 0.39f, 0.45f), 0.5f);


    PointLight pointLight(glm::vec3(4.0f, 8.0f, 4.0f), glm::vec3(0.29f, 0.29f, 0.3f),
        glm::vec3(0.29f, 0.29f, 0.3f), glm::vec3(0.29f, 0.29f, 0.3f));

    // scene.addLight(dirLight);
    // scene.addLight(pointLight);

    ShadowMap shadowMap(2048.0, 2048.0);


    Carbon::FrameBuffer framebuffer(windowWidth, windowHeight);

    float rectangle[] = {
        // First Triangle
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        // Second Triangle
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 1.0f
    };

    unsigned int quadVAO, rectVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &rectVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), rectangle, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    static float historicMinRenderTime = std::numeric_limits<float>::max();
    static float historicMaxRenderTime = 0.0f;

    static const int historySize = 120;
    static float shadowPassHistory[historySize] = {0.0f};
    static int historyIndexShadow = 0;
    static float renderHistory[historySize] = {0.0f};
    static int historyIndexRender = 0;

    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);

    while (!glfwWindowShouldClose(window))
    {
        camera.setAspectRatio(aspectRatio);

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
                    camera.processKeyboard("FORWARD", deltaTime);
                if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                    camera.processKeyboard("BACKWARD", deltaTime);
                if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                    camera.processKeyboard("LEFT", deltaTime);
                if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                    camera.processKeyboard("RIGHT", deltaTime);

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

                camera.processMouseMovement(xOffset, yOffset, true);
            }
            else
            {
                // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }

        // TODO fix this as it only takes in the directional light atm
        glm::mat4 lightSpaceMatrix = shadowMap.CalculateLightSpaceMatrix(dirLight.getDirection());

        renderer.ShadowPass(scene.getRegistry(), shaderManager, shadowMap, lightSpaceMatrix);

        // glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);
        // std::cout << "Current FBO pre-render: " << currentFBO << std::endl;


        framebuffer.Bind();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
        glEnable(GL_DEPTH_TEST);


        // shader and set uniforms
        lightingShader->Bind();
        lightingShader->SetUniformMat4f("view", camera.getViewMatrix());
        lightingShader->SetUniformMat4f("projection", camera.getProjectionMatrix());
        lightingShader->SetUniform3f("viewPos", camera.getPosition());

        // all of this lighting information should be inside the scene or something else that can be accessed in the renderer
        lightingShader->SetUniform3f("light.direction", dirLight.getDirection());
        lightingShader->SetUniform3f("light.ambient", dirLight.getAmbient());
        lightingShader->SetUniform3f("light.diffuse", dirLight.getDiffuse());
        lightingShader->SetUniform3f("light.specular", dirLight.getSpecular());
        lightingShader->SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, shadowMap.GetDepthTexture());
        lightingShader->SetUniform1i("shadowMap", 4);
        lightingShader->SetUniform2f("gMapSize", glm::vec2(2048.0f, 2048.0f));

        renderer.Render(scene.getRegistry(), shaderManager, lightingShader);


        framebuffer.Unbind();

        framebufferShader->Bind();
        glBindVertexArray(quadVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, framebuffer.GetTextureColorBuffer());
        framebufferShader->SetUniform1i("framebuf", 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);


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
            unsigned int textureID = framebuffer.GetTextureColorBuffer();
            ImGui::Image(textureID, viewportPanelSize, ImVec2{0, 1}, ImVec2{1, 0});
            ImGui::PopStyleVar();
            ImGui::End();
        }
        // Rendering
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

    window = glfwCreateWindow(1920*2, 1080*2, "Carbon Renderer", glfwGetPrimaryMonitor(), nullptr);
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
