#include "VertexBuffer.h"

#include <array>
#include <cstring>

void VertexBuffer::Create(
    vk::raii::PhysicalDevice& physicalDevice,
    vk::raii::Device& device)
{
    constexpr std::array<Vertex, 8> vertices =
    { {
        { -0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f, -0.5f },
        {  0.5f,  0.5f, -0.5f },
        { -0.5f,  0.5f, -0.5f },

        { -0.5f, -0.5f,  0.5f },
        {  0.5f, -0.5f,  0.5f },
        {  0.5f,  0.5f,  0.5f },
        { -0.5f,  0.5f,  0.5f }
    } };

    vertexCount = static_cast<uint32_t>(vertices.size());

    buffer.Create(
        physicalDevice,
        device,
        sizeof(vertices),
        vk::BufferUsageFlagBits::eVertexBuffer,
        vk::MemoryPropertyFlagBits::eHostVisible |
        vk::MemoryPropertyFlagBits::eHostCoherent
    );

    void* data = buffer.GetMemory().mapMemory(
        0,
        sizeof(vertices)
    );

    std::memcpy(
        data,
        vertices.data(),
        sizeof(vertices)
    );

    buffer.GetMemory().unmapMemory();
}

void VertexBuffer::Cleanup()
{
    buffer.Cleanup();
    vertexCount = 0;
}

vk::raii::Buffer& VertexBuffer::Get()
{
    return buffer.Get();
}

uint32_t VertexBuffer::GetVertexCount() const
{
    return vertexCount;
}