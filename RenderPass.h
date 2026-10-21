#pragma once
#include "Dependancies.h"

class RenderPass
{
public:
    void Create(vk::raii::Device& device, vk::Format imageFormat);
    void Cleanup();
    vk::raii::RenderPass& Get();

private:
    vk::raii::RenderPass handle = nullptr;
};
