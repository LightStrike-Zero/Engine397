#include "GLFWWindow.h"
#include <iostream>

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
