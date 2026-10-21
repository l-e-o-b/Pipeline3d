#pragma once

#include "Dependancies.h"
#include "UniformBuffer.h"

class DescriptorManager
{
public:
    void Create(
        vk::raii::Device& device,
        UniformBuffer& uniformBuffer);

    void Cleanup();

    vk::raii::DescriptorSet& Get();
    vk::raii::DescriptorSetLayout& GetLayout();


private:
    vk::raii::DescriptorSetLayout layout = nullptr;
    vk::raii::DescriptorPool pool = nullptr;
    vk::raii::DescriptorSet descriptorSet = nullptr;
};