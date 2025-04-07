
#include "ImGui_UI.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void ImGuiUI::Initialise(GLFWwindow* window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Setup style
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiUI::BeginFrame()
{
    // Start the frame for both backends and the ImGui context
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // Setup docking space over the viewport (if needed)
    ImGui::DockSpaceOverViewport();
}

void ImGuiUI::DisplayImage(const char* title, const unsigned int imagePtr, const glm::vec2& imageSize)
{
    // Create a dockable window and display the image.
    ImGui::Begin(title);
    ImVec2 availableSize = ImGui::GetContentRegionAvail();

    float offsetX = (availableSize.x - imageSize.x) * 0.5f;
    float offsetY = (availableSize.y - imageSize.y) * 0.5f;

    if (offsetX < 0.0f) offsetX = 0.0f;
    if (offsetY < 0.0f) offsetY = 0.0f;

    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + offsetX, ImGui::GetCursorPosY() + offsetY));

    // Note: Adjust UV coordinates if needed (here we flip vertically)
    ImGui::Image(imagePtr, ImVec2(imageSize.x, imageSize.y), ImVec2{0, 1}, ImVec2{1, 0});
    ImGui::End();
}

void ImGuiUI::EndFrame()
{
    // Finalize the frame and render
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and render additional platform windows if viewports are enabled.
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void ImGuiUI::Shutdown()
{
    // Clean up all resources allocated by ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
