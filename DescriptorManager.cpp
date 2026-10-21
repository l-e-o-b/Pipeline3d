#include "DescriptorManager.h"
#include <iostream>

void DescriptorManager::Create(
    vk::raii::Device& device,
    UniformBuffer& uniformBuffer)
{
    vk::DescriptorSetLayoutBinding uboBinding{};
    uboBinding.binding = 0;
    uboBinding.descriptorType =
        vk::DescriptorType::eUniformBuffer;
    uboBinding.descriptorCount = 1;
    uboBinding.stageFlags =
        vk::ShaderStageFlagBits::eVertex;

    vk::DescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboBinding;
    std::cout << "DEBUG 1\n";
    layout = vk::raii::DescriptorSetLayout(
        device,
        layoutInfo
    );
    std::cout << "DEBUG 2\n";
    vk::DescriptorPoolSize poolSize{};
    poolSize.type = vk::DescriptorType::eUniformBuffer;
    poolSize.descriptorCount = 1;

    vk::DescriptorPoolCreateInfo poolInfo{};
    poolInfo.maxSets = 1;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    std::cout << "DEBUG 3\n";
    pool = vk::raii::DescriptorPool(
        device,
        poolInfo
    );
    std::cout << "DEBUG 4\n";
    vk::DescriptorSetAllocateInfo allocateInfo{};
    allocateInfo.descriptorPool = *pool;
    allocateInfo.descriptorSetCount = 1;
    allocateInfo.pSetLayouts = &*layout;

    std::cout << "DEBUG 5\n";
    auto sets = device.allocateDescriptorSets(allocateInfo);
    descriptorSet = std::move(sets[0]);
    std::cout << "DEBUG 6\n";
    vk::DescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = *uniformBuffer.Get();
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObject);
    std::cout << "DEBUG 7\n";
    vk::WriteDescriptorSet write{};
    write.dstSet = *descriptorSet;
    write.dstBinding = 0;
    write.dstArrayElement = 0;
    write.descriptorCount = 1;
    write.descriptorType =
        vk::DescriptorType::eUniformBuffer;
    write.pBufferInfo = &bufferInfo;

    device.updateDescriptorSets(
        write,
        nullptr
    );
    std::cout << "DEBUG 8\n";
}

void DescriptorManager::Cleanup()
{
    descriptorSet = nullptr;
    pool = nullptr;
    layout = nullptr;
}

vk::raii::DescriptorSet& DescriptorManager::Get()
{
    return descriptorSet;
}

vk::raii::DescriptorSetLayout& DescriptorManager::GetLayout()
{
    return layout;
}