#pragma once

#include "Buffer.h"

struct Vertex
{
    float x;
    float y;
    float z;
};

class VertexBuffer
{
public:
    void Create(
        vk::raii::PhysicalDevice& physicalDevice,
        vk::raii::Device& device);

    void Cleanup();

    vk::raii::Buffer& Get();
    uint32_t GetVertexCount() const;

private:
    Buffer buffer;
    uint32_t vertexCount = 0;
};