#pragma once

#include "Dependancies.h"

class Buffer
{
public:
    void Create(
        vk::raii::PhysicalDevice& physicalDevice,
        vk::raii::Device& device,
        vk::DeviceSize size,
        vk::BufferUsageFlags usage,
        vk::MemoryPropertyFlags properties);
        vk::raii::DeviceMemory& GetMemory();

    void Cleanup();

    vk::raii::Buffer& Get();

private:
    vk::raii::Buffer buffer = nullptr;
    vk::raii::DeviceMemory memory = nullptr;
};