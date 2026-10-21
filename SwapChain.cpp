#include "Swapchain.h"
#include <algorithm>
#include <array>
#include <limits>
#include <stdexcept>

namespace
{
    vk::SurfaceFormatKHR ChooseFormat(const std::vector<vk::SurfaceFormatKHR>& formats)
    {
        for (const auto& format : formats)
            if (format.format == vk::Format::eB8G8R8A8Srgb &&
                format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
                return format;
        return formats.front();
    }

    vk::CompositeAlphaFlagBitsKHR ChooseAlpha(vk::CompositeAlphaFlagsKHR supported)
    {
        constexpr std::array options{
            vk::CompositeAlphaFlagBitsKHR::eOpaque,
            vk::CompositeAlphaFlagBitsKHR::ePreMultiplied,
            vk::CompositeAlphaFlagBitsKHR::ePostMultiplied,
            vk::CompositeAlphaFlagBitsKHR::eInherit };
        for (const auto option : options)
            if (static_cast<bool>(supported & option)) return option;
        throw std::runtime_error("Aucun mode composite alpha disponible.");
    }

    uint32_t ClampExtent(uint32_t desired, uint32_t minimum, uint32_t maximum)
    {
        return std::min(std::max(desired, minimum), maximum);
    }
}

void Swapchain::Create(
    vk::raii::PhysicalDevice& physicalDevice, vk::raii::Device& device,
    vk::raii::SurfaceKHR& surface, GLFWwindow* window,
    uint32_t graphicsFamily, uint32_t presentFamily)
{
    const auto capabilities = physicalDevice.getSurfaceCapabilitiesKHR(*surface);
    const auto formats = physicalDevice.getSurfaceFormatsKHR(*surface);
    if (formats.empty()) throw std::runtime_error("Aucun format de surface.");

    const auto selectedFormat = ChooseFormat(formats);
    imageFormat = selectedFormat.format;
    extent = capabilities.currentExtent;
    if (extent.width == std::numeric_limits<uint32_t>::max())
    {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        extent.width = ClampExtent(static_cast<uint32_t>(std::max(width, 1)),
            capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        extent.height = ClampExtent(static_cast<uint32_t>(std::max(height, 1)),
            capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    }

    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0)
        imageCount = std::min(imageCount, capabilities.maxImageCount);

    const std::array queueFamilies{ graphicsFamily, presentFamily };
    vk::SwapchainCreateInfoKHR info{};
    info.surface = *surface;
    info.minImageCount = imageCount;
    info.imageFormat = selectedFormat.format;
    info.imageColorSpace = selectedFormat.colorSpace;
    info.imageExtent = extent;
    info.imageArrayLayers = 1;
    info.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
    info.preTransform = capabilities.currentTransform;
    info.compositeAlpha = ChooseAlpha(capabilities.supportedCompositeAlpha);
    info.presentMode = vk::PresentModeKHR::eFifo;
    info.clipped = VK_TRUE;

    if (graphicsFamily != presentFamily)
    {
        info.imageSharingMode = vk::SharingMode::eConcurrent;
        info.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilies.size());
        info.pQueueFamilyIndices = queueFamilies.data();
    }
    else info.imageSharingMode = vk::SharingMode::eExclusive;

    handle = vk::raii::SwapchainKHR(device, info);
    images = handle.getImages();
    CreateImageViews(device);
}

void Swapchain::CreateImageViews(vk::raii::Device& device)
{
    imageViews.clear();
    for (const auto image : images)
    {
        vk::ImageViewCreateInfo info{};
        info.image = image;
        info.viewType = vk::ImageViewType::e2D;
        info.format = imageFormat;
        info.components.r = vk::ComponentSwizzle::eIdentity;
        info.components.g = vk::ComponentSwizzle::eIdentity;
        info.components.b = vk::ComponentSwizzle::eIdentity;
        info.components.a = vk::ComponentSwizzle::eIdentity;
        info.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        info.subresourceRange.levelCount = 1;
        info.subresourceRange.layerCount = 1;
        imageViews.emplace_back(device, info);
    }
}

void Swapchain::Cleanup()
{
    imageViews.clear();
    handle = nullptr;
    images.clear();
}
vk::raii::SwapchainKHR& Swapchain::GetHandle() { return handle; }
const std::vector<vk::Image>& Swapchain::GetImages() const { return images; }
const std::vector<vk::raii::ImageView>& Swapchain::GetImageViews() const { return imageViews; }
vk::Format Swapchain::GetImageFormat() const { return imageFormat; }
vk::Extent2D Swapchain::GetExtent() const { return extent; }
