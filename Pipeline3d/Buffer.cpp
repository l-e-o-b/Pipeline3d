#include "Buffer.h"
#include <stdexcept>
#include <cstdint>

void Buffer::Create(
    vk::raii::PhysicalDevice& physicalDevice,
    vk::raii::Device& device,
    vk::DeviceSize size,
    vk::BufferUsageFlags usage,
    vk::MemoryPropertyFlags properties)
{
    vk::BufferCreateInfo bufferInfo{};
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = vk::SharingMode::eExclusive;

    buffer = vk::raii::Buffer(device, bufferInfo);

    const auto requirements = buffer.getMemoryRequirements();

    uint32_t memoryType = UINT32_MAX;

    const auto memoryProperties =
        physicalDevice.getMemoryProperties();

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
    {
        if ((requirements.memoryTypeBits & (1 << i)) &&
            (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            memoryType = i;
            break;
        }
    }

    if (memoryType == UINT32_MAX)
    {
        throw std::runtime_error(
            "Type de mémoire Vulkan introuvable.");
    }

    vk::MemoryAllocateInfo allocateInfo{};
    allocateInfo.allocationSize = requirements.size;
    allocateInfo.memoryTypeIndex = memoryType;

    memory = vk::raii::DeviceMemory(device, allocateInfo);

    buffer.bindMemory(*memory, 0);
}

void Buffer::Cleanup()
{
    memory = nullptr;
    buffer = nullptr;
}

vk::raii::Buffer& Buffer::Get()
{
    return buffer;
}

vk::raii::DeviceMemory& Buffer::GetMemory()
{
    return memory;
}