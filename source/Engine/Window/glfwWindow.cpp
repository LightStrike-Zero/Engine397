#include "GLFWWindow.h"
#include <iostream>
#include "Systems/EventSystem.h"

GLFWWindow::GLFWWindow(int width, int height, const std::string& title, bool fullscreen) {
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed" << std::endl;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;
    m_window = glfwCreateWindow(width, height, title.c_str(), monitor, nullptr);
    if (!m_window) {
        std::cerr << "GLFW window creation failed" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr << "glewInit(): GLEW failed to initialize" << std::endl;
    }
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

GLFWWindow::~GLFWWindow() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool GLFWWindow::ShouldClose() const {
    return glfwWindowShouldClose(m_window);
}

// buko - need tis to shut down after splash image is clicked on
void GLFWWindow::SetShouldClose(bool value)
{
    glfwSetWindowShouldClose(m_window, value);
}

void GLFWWindow::PollEvents() {
    glfwPollEvents();
}

void GLFWWindow::SwapBuffers() {
    glfwSwapBuffers(m_window);
}

void GLFWWindow::GetFramebufferSize(int &width, int &height) const {
    glfwGetFramebufferSize(m_window, &width, &height);
}

void GLFWWindow::SetInputMode(int mode, int value) {
    glfwSetInputMode(m_window, mode, value);
}

void* GLFWWindow::GetNativeWindow() const {
    return static_cast<void*>(m_window);
}

void GLFWWindow::pollInputEvents() {
    //exit window
    if (glfwGetKey(m_window, GLFW_KEY_X) == GLFW_PRESS) {
        EventSystem::getInstance().dispatchEvent(KeyPressedEvent(GLFW_KEY_X));
    }
    //toggle line mode
    if (glfwGetKey(m_window, GLFW_KEY_K) == GLFW_PRESS) {
        EventSystem::getInstance().dispatchEvent(KeyPressedEvent(GLFW_KEY_K));
    }
    //get manual
    if (glfwGetKey(m_window, GLFW_KEY_M) == GLFW_PRESS) {
        EventSystem::getInstance().dispatchEvent(KeyPressedEvent(GLFW_KEY_M));
    }
    //space for shoot
    if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        EventSystem::getInstance().dispatchEvent(KeyPressedEvent(GLFW_KEY_M));
    }

    //mouse for shoot/jump?
    //if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS) {}


}
