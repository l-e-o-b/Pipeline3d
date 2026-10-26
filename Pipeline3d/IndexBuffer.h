#pragma once

#include "Buffer.h"

class IndexBuffer
{
public:
    void Create(
        vk::raii::PhysicalDevice& physicalDevice,
        vk::raii::Device& device);

    void Cleanup();

    vk::raii::Buffer& Get();
    uint32_t GetIndexCount() const;

private:
    Buffer buffer;
    uint32_t indexCount = 0;
};