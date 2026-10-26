#pragma once
#include <vector>
#include "Dependancies.h"

class FramebufferManager
{
public:
    void Create(vk::raii::Device& device, vk::raii::RenderPass& renderPass,
        const std::vector<vk::raii::ImageView>& imageViews, vk::Extent2D extent);
    void Cleanup();
    const std::vector<vk::raii::Framebuffer>& GetAll() const;

private:
    std::vector<vk::raii::Framebuffer> framebuffers;
};
