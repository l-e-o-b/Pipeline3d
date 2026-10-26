#include "Framebuffer.h"

void FramebufferManager::Create(vk::raii::Device& device,
    vk::raii::RenderPass& renderPass,
    const std::vector<vk::raii::ImageView>& imageViews, vk::Extent2D extent)
{
    framebuffers.clear();
    for (const auto& imageView : imageViews)
    {
        const vk::ImageView attachment = *imageView;
        vk::FramebufferCreateInfo info{};
        info.renderPass = *renderPass;
        info.attachmentCount = 1;
        info.pAttachments = &attachment;
        info.width = extent.width;
        info.height = extent.height;
        info.layers = 1;
        framebuffers.emplace_back(device, info);
    }
}

void FramebufferManager::Cleanup() { framebuffers.clear(); }
const std::vector<vk::raii::Framebuffer>& FramebufferManager::GetAll() const
{
    return framebuffers;
}
