#include "CommandManager.h"
#include <iostream>

void CommandManager::Create(vk::raii::Device& device,
    uint32_t queueFamilyIndex,
    const std::vector<vk::raii::Framebuffer>& framebuffers,
    vk::raii::RenderPass& renderPass,
    vk::raii::Pipeline& pipeline,
    vk::raii::PipelineLayout& pipelineLayout,
    vk::Extent2D extent,
    VertexBuffer& vertexBuffer,
    IndexBuffer& indexBuffer,
    vk::raii::DescriptorSet& descriptorSet)
{
    vk::CommandPoolCreateInfo poolInfo{};
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    poolInfo.queueFamilyIndex = queueFamilyIndex;
    pool = vk::raii::CommandPool(device, poolInfo);

    vk::CommandBufferAllocateInfo allocInfo{};
    allocInfo.commandPool = *pool;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandBufferCount = static_cast<uint32_t>(framebuffers.size());
    buffers = device.allocateCommandBuffers(allocInfo);

    for (size_t i = 0; i < buffers.size(); ++i)
    {
        buffers[i].begin(vk::CommandBufferBeginInfo{});
        vk::ClearValue clear{};
        clear.color.float32[0] = 0.02f;
        clear.color.float32[1] = 0.02f;
        clear.color.float32[2] = 0.04f;
        clear.color.float32[3] = 1.0f;

        vk::RenderPassBeginInfo renderInfo{};
        renderInfo.renderPass = *renderPass;
        renderInfo.framebuffer = *framebuffers[i];
        renderInfo.renderArea.extent = extent;
        renderInfo.clearValueCount = 1;
        renderInfo.pClearValues = &clear;
        buffers[i].beginRenderPass(renderInfo, vk::SubpassContents::eInline);
        buffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, *pipeline);
        std::cout << "AVANT BIND DESCRIPTOR\n";

        vk::DescriptorSet rawDescriptorSet = *descriptorSet;

        std::cout << "APRES RECUP DESCRIPTOR\n";

        buffers[i].bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics,
            *pipelineLayout,
            0,
            rawDescriptorSet,
            {}
        );

        std::cout << "APRES BIND DESCRIPTOR\n";
        std::cout << "AVANT VERTEX BUFFER\n";

        vk::DeviceSize offset = 0;

        buffers[i].bindVertexBuffers(
            0,
            *vertexBuffer.Get(),
            offset
        );

        std::cout << "APRES VERTEX BUFFER\n";

        buffers[i].bindIndexBuffer(
            *indexBuffer.Get(),
            0,
            vk::IndexType::eUint32
        );

        std::cout << "APRES INDEX BUFFER\n";

        buffers[i].drawIndexed(
            indexBuffer.GetIndexCount(),
            1,
            0,
            0,
            0
        );
        buffers[i].endRenderPass();
        buffers[i].end();
    }
}

void CommandManager::Cleanup()
{
    buffers.clear();
    pool = nullptr;
}
std::vector<vk::raii::CommandBuffer>& CommandManager::GetAll() { return buffers; }
