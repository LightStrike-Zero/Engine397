#include "ImGui_UI.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "texture/TextureManager.h" //buko
//#include "Interfaces/IWindow.h" // buko

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

//buko - need this to flip the splash image the wright way and also make it clickable
bool ImGuiUI::DisplayClickableImageFlipped(const char* title, const unsigned int imagePtr, const glm::vec2& imageSize)
{
    // Create a dockable window and display the image.
    ImGui::Begin(title, nullptr,
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoBackground |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoScrollWithMouse |
                 ImGuiWindowFlags_NoDocking);
    ImVec2 availableSize = ImGui::GetContentRegionAvail();

    float offsetX = (availableSize.x - imageSize.x) * 0.5f;
    float offsetY = (availableSize.y - imageSize.y) * 0.5f;

    if (offsetX < 0.0f) offsetX = 0.0f;
    if (offsetY < 0.0f) offsetY = 0.0f;

    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + offsetX, ImGui::GetCursorPosY() + offsetY));

    // Note: Adjust UV coordinates if needed (here we flip vertically)
    ImGui::Image(imagePtr, ImVec2(imageSize.x, imageSize.y), ImVec2{0, 0}, ImVec2{1, 1});

    bool clicked = ImGui::IsItemClicked();

    ImGui::End();

    return clicked;
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

//buko loads the splash image
void ImGuiUI::loadNamedImage(const std::string& name, const std::string& path)
{
    unsigned int id = TextureManager::getInstance().loadTextureFromFile(path);
    m_namedImages[name] = id;
}

//buko - modified by shaun to remove begin/end calls
bool ImGuiUI::showNamedClickableImage(const std::string& name, const glm::vec2& size)
{
    auto it = m_namedImages.find(name);
    if (it == m_namedImages.end())
        return false;

    // old two‐arg wrap:
    // return ImageButton(it->second, size);

    // new three‐arg call: use the asset name as the ImGui ID
    return ImageButton(name.c_str(), it->second, size);
}

// buko shows the help manual
void ImGuiUI::ShowHelpManual(bool& show, const std::string& helpText)
{
    if (!show) return;

    const ImVec2 helpSize = ImVec2(800, 600); // Set a good size
    const ImVec2 helpPos = ImVec2(560, 240); // Position it somewhere comfy

    ImGui::SetNextWindowPos(helpPos, ImGuiCond_Once);
    ImGui::SetNextWindowSize(helpSize, ImGuiCond_Once);

    ImGui::Begin("Help Manual", &show,
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoResize);

    ImGui::TextWrapped("%s", helpText.c_str());

    ImGui::End();
}

void ImGuiUI::BeginWindow(const char* name, bool* p_open, ImGuiWindowFlags flags)
{
    ImGui::Begin(name, p_open, flags);
}

void ImGuiUI::EndWindow()
{
    ImGui::End();
}

void ImGuiUI::ImageWidget(unsigned int texID, const glm::vec2& size)
{
    ImGui::Image(reinterpret_cast<ImTextureID>(reinterpret_cast<void*>(static_cast<intptr_t>(texID))),
                 ImVec2(size.x, size.y), ImVec2{0, 1}, ImVec2{1, 0});
}

bool ImGuiUI::ImageButton(const char* id,
                          unsigned int texID,
                          const glm::vec2& size)
{
    ImTextureID img = static_cast<ImTextureID>(static_cast<uintptr_t>(texID));
    return ImGui::ImageButton(
        id, // unique string ID
        img, // your GL texture handle
        ImVec2(size.x, size.y), // size
        ImVec2(0, 1), ImVec2(1, 0) // flipped UVs
    );
}
