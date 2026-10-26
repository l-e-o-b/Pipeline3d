#pragma once
#include <cstddef>
#include <vector>
#include "Dependancies.h"

class SyncObjects
{
public:
    void Create(vk::raii::Device& device, std::size_t imageCount);
    void Cleanup();
    vk::raii::Semaphore& GetImageAvailable();
    vk::raii::Semaphore& GetRenderFinished(std::size_t imageIndex);
    vk::raii::Fence& GetInFlightFence();

private:
    vk::raii::Semaphore imageAvailable = nullptr;
    std::vector<vk::raii::Semaphore> renderFinished;
    vk::raii::Fence inFlightFence = nullptr;
};
