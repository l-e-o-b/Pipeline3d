#include "Application.h"

#include <stdexcept>

void Application::InitWindow()
{
    if (!glfwInit()) throw std::runtime_error("Impossible d'initialiser GLFW.");
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(width, height, "Pipeline3d - Triangle Vulkan", nullptr, nullptr);
    if (!window) throw std::runtime_error("Impossible de creer la fenetre GLFW.");
}

void Application::Init()
{
    InitWindow();
    renderer.Init(window);
}

void Application::Loop()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        renderer.DrawFrame();
    }
}

void Application::DestroyWindow()
{
    if (window) glfwDestroyWindow(window);
    window = nullptr;
    glfwTerminate();
}

void Application::Destroy()
{
    renderer.Cleanup();
    DestroyWindow();
}
