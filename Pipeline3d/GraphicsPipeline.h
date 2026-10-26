#pragma once
#include "Dependancies.h"
#include "VertexBuffer.h"

class GraphicsPipeline
{
public:
    void Create(vk::raii::Device& device, vk::raii::RenderPass& renderPass,
        vk::Extent2D extent, vk::raii::DescriptorSetLayout& descriptorSetLayout);
    void Cleanup();
    vk::raii::Pipeline& Get();
    vk::raii::PipelineLayout& GetLayout();

private:
    vk::raii::PipelineLayout layout = nullptr;
    vk::raii::Pipeline pipeline = nullptr;
};
