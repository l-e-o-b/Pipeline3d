#include "UniformBuffer.h"

#include <cstring>

void UniformBuffer::Create(
    vk::raii::PhysicalDevice& physicalDevice,
    vk::raii::Device& device)
{
    buffer.Create(
        physicalDevice,
        device,
        sizeof(UniformBufferObject),
        vk::BufferUsageFlagBits::eUniformBuffer,
        vk::MemoryPropertyFlagBits::eHostVisible |
        vk::MemoryPropertyFlagBits::eHostCoherent
    );
}

void UniformBuffer::Update(
    vk::raii::Device& device,
    const UniformBufferObject& ubo)
{
    void* data = buffer.GetMemory().mapMemory(
        0,
        sizeof(UniformBufferObject)
    );

    std::memcpy(
        data,
        &ubo,
        sizeof(UniformBufferObject)
    );

    buffer.GetMemory().unmapMemory();
}

void UniformBuffer::Cleanup()
{
    buffer.Cleanup();
}

vk::raii::Buffer& UniformBuffer::Get()
{
    return buffer.Get();
}