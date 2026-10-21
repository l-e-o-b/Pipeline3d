#pragma once
#include "Renderer.h"

class Application
{
public:
    void Init();
    void Loop();
    void Destroy();

private:
    static constexpr int width = 800;
    static constexpr int height = 600;
    GLFWwindow* window = nullptr;
    Renderer renderer;

    void InitWindow();
    void DestroyWindow();
};
