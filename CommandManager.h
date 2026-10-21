#pragma once
#include <vector>
#include "Dependancies.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class CommandManager
{
public:
    void Create(
        vk::raii::Device& device,
        uint32_t queueFamilyIndex,
        const std::vector<vk::raii::Framebuffer>& framebuffers,
        vk::raii::RenderPass& renderPass,
        vk::raii::Pipeline& pipeline,
        vk::raii::PipelineLayout& pipelineLayout,
        vk::Extent2D extent,
        VertexBuffer& vertexBuffer,
        IndexBuffer& indexBuffer,
        vk::raii::DescriptorSet& descriptorSet);

    void Cleanup();
    std::vector<vk::raii::CommandBuffer>& GetAll();

private:
    vk::raii::CommandPool pool = nullptr;
    std::vector<vk::raii::CommandBuffer> buffers;
};
