#include "IndexBuffer.h"

#include <array>
#include <cstring>

void IndexBuffer::Create(
    vk::raii::PhysicalDevice& physicalDevice,
    vk::raii::Device& device)
{
    constexpr std::array<uint32_t, 36> indices =
    { {
            // arrière
            0, 1, 2,
            2, 3, 0,
            // Avant
            4, 6, 5,
            6, 4, 7,

            // gauche
            0, 3, 7,
            7, 4, 0,

            // droite
            1, 5, 6,
            6, 2, 1,

            // bas
            0, 4, 5,
            5, 1, 0,

            // haut
            3, 2, 6,
            6, 7, 3
        } };

    indexCount = static_cast<uint32_t>(indices.size());

    buffer.Create(
        physicalDevice,
        device,
        sizeof(indices),
        vk::BufferUsageFlagBits::eIndexBuffer,
        vk::MemoryPropertyFlagBits::eHostVisible |
        vk::MemoryPropertyFlagBits::eHostCoherent
    );

    void* data = buffer.GetMemory().mapMemory(
        0,
        sizeof(indices)
    );

    std::memcpy(
        data,
        indices.data(),
        sizeof(indices)
    );

    buffer.GetMemory().unmapMemory();
}

void IndexBuffer::Cleanup()
{
    buffer.Cleanup();
    indexCount = 0;
}

vk::raii::Buffer& IndexBuffer::Get()
{
    return buffer.Get();
}

uint32_t IndexBuffer::GetIndexCount() const
{
    return indexCount;
}