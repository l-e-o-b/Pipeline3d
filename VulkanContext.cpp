#include "VulkanContext.h"
#include <iostream>
#include <stdexcept>
#include <vector>

void VulkanContext::Create(GLFWwindow* window)
{
    CreateInstance();
    CreateSurface(window);
    PickPhysicalDevice();
    CreateLogicalDevice();
}

void VulkanContext::CreateInstance()
{
    vk::ApplicationInfo appInfo{};
    appInfo.pApplicationName = "Pipeline3d";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Pipeline3d";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = vk::ApiVersion10;

    uint32_t extensionCount = 0;
    const char** requiredExtensions =
        glfwGetRequiredInstanceExtensions(&extensionCount);

    if (!requiredExtensions)
        throw std::runtime_error("Extensions Vulkan GLFW indisponibles.");

    std::vector<const char*> extensions(
        requiredExtensions,
        requiredExtensions + extensionCount);

    vk::InstanceCreateInfo info{};
    info.pApplicationInfo = &appInfo;
    info.enabledExtensionCount =
        static_cast<uint32_t>(extensions.size());
    info.ppEnabledExtensionNames = extensions.data();

    instance = vk::raii::Instance(context, info);
}

void VulkanContext::CreateSurface(GLFWwindow* window)
{
    VkSurfaceKHR rawSurface = VK_NULL_HANDLE;

    if (glfwCreateWindowSurface(
        static_cast<VkInstance>(*instance),
        window,
        nullptr,
        &rawSurface) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "Impossible de creer la surface Vulkan.");
    }

    surface = vk::raii::SurfaceKHR(instance, rawSurface);
}

void VulkanContext::PickPhysicalDevice()
{
    const auto devices = instance.enumeratePhysicalDevices();

    if (devices.empty())
        throw std::runtime_error(
            "Aucun GPU compatible Vulkan detecte.");

    for (const auto& candidate : devices)
    {
        const auto families =
            candidate.getQueueFamilyProperties();

        for (uint32_t i = 0; i < families.size(); ++i)
        {
            const bool graphics =
                families[i].queueCount > 0 &&
                static_cast<bool>(
                    families[i].queueFlags &
                    vk::QueueFlagBits::eGraphics);

            const bool present =
                graphics &&
                static_cast<bool>(
                    candidate.getSurfaceSupportKHR(
                        i, *surface));

            if (!present)
                continue;

            physicalDevice = candidate;
            graphicsQueueFamilyIndex = i;
            presentQueueFamilyIndex = i;

            std::cout << "GPU selectionne : "
                << physicalDevice.getProperties().deviceName
                << '\n';

            return;
        }
    }

    throw std::runtime_error(
        "Aucun GPU avec une queue graphique et de presentation.");
}

void VulkanContext::CreateLogicalDevice()
{
    const float priority = 1.0f;

    vk::DeviceQueueCreateInfo queueInfo{};
    queueInfo.queueFamilyIndex =
        graphicsQueueFamilyIndex;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &priority;

    const char* extensions[] =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    vk::DeviceCreateInfo info{};
    info.queueCreateInfoCount = 1;
    info.pQueueCreateInfos = &queueInfo;
    info.enabledExtensionCount = 1;
    info.ppEnabledExtensionNames = extensions;

    device = vk::raii::Device(
        physicalDevice,
        info);

    deviceCreated = true;

    graphicsQueue =
        vk::raii::Queue(
            device,
            graphicsQueueFamilyIndex,
            0);

    presentQueue =
        vk::raii::Queue(
            device,
            presentQueueFamilyIndex,
            0);
}

void VulkanContext::Cleanup()
{
    graphicsQueue = nullptr;
    presentQueue = nullptr;
    device = nullptr;
    physicalDevice = nullptr;
    surface = nullptr;
    instance = nullptr;
    deviceCreated = false;
}

bool VulkanContext::HasDevice() const
{
    return deviceCreated;
}

uint32_t VulkanContext::FindMemoryType(
    uint32_t typeFilter,
    vk::MemoryPropertyFlags properties) const
{
    const auto memoryProperties =
        physicalDevice.getMemoryProperties();

    for (uint32_t i = 0;
        i < memoryProperties.memoryTypeCount;
        ++i)
    {
        if ((typeFilter & (1 << i)) &&
            (memoryProperties.memoryTypes[i].propertyFlags &
                properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error(
        "Type de mémoire Vulkan introuvable.");
}

vk::raii::Instance&
VulkanContext::GetInstance()
{
    return instance;
}

vk::raii::SurfaceKHR&
VulkanContext::GetSurface()
{
    return surface;
}

vk::raii::PhysicalDevice&
VulkanContext::GetPhysicalDevice()
{
    return physicalDevice;
}

vk::raii::Device&
VulkanContext::GetDevice()
{
    return device;
}

vk::raii::Queue&
VulkanContext::GetGraphicsQueue()
{
    return graphicsQueue;
}

vk::raii::Queue&
VulkanContext::GetPresentQueue()
{
    return presentQueue;
}

uint32_t VulkanContext::GetGraphicsQueueFamilyIndex() const
{
    return graphicsQueueFamilyIndex;
}

uint32_t VulkanContext::GetPresentQueueFamilyIndex() const
{
    return presentQueueFamilyIndex;
}