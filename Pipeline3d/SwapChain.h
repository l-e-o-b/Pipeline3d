#pragma once

#include <cstdint>
#include <vector>
#include "Dependancies.h"

class Swapchain
{
public:
    void Create(
        vk::raii::PhysicalDevice& physicalDevice,
        vk::raii::Device& device,
        vk::raii::SurfaceKHR& surface,
        GLFWwindow* window,
        uint32_t graphicsQueueFamilyIndex,
        uint32_t presentQueueFamilyIndex);
    void Cleanup();

    vk::raii::SwapchainKHR& GetHandle();
    const std::vector<vk::Image>& GetImages() const;
    const std::vector<vk::raii::ImageView>& GetImageViews() const;
    vk::Format GetImageFormat() const;
    vk::Extent2D GetExtent() const;

private:
    vk::raii::SwapchainKHR handle = nullptr;
    std::vector<vk::Image> images;
    std::vector<vk::raii::ImageView> imageViews;
    vk::Format imageFormat{};
    vk::Extent2D extent{};

    void CreateImageViews(vk::raii::Device& device);
};
