#pragma once

#include <cstdint>
#include "Dependancies.h"


class VulkanContext
{
public:
    void Create(GLFWwindow* window);
    void Cleanup();
    bool HasDevice() const;

    vk::raii::Instance& GetInstance();
    vk::raii::SurfaceKHR& GetSurface();
    vk::raii::PhysicalDevice& GetPhysicalDevice();
    vk::raii::Device& GetDevice();
    vk::raii::Queue& GetGraphicsQueue();
    vk::raii::Queue& GetPresentQueue();
    uint32_t GetGraphicsQueueFamilyIndex() const;
    uint32_t GetPresentQueueFamilyIndex() const;
    uint32_t FindMemoryType(
        uint32_t typeFilter,
        vk::MemoryPropertyFlags properties) const;

private:
    vk::raii::Context context;
    vk::raii::Instance instance = nullptr;
    vk::raii::SurfaceKHR surface = nullptr;
    vk::raii::PhysicalDevice physicalDevice = nullptr;
    vk::raii::Device device = nullptr;
    vk::raii::Queue graphicsQueue = nullptr;
    vk::raii::Queue presentQueue = nullptr;
    uint32_t graphicsQueueFamilyIndex = 0;
    uint32_t presentQueueFamilyIndex = 0;
    bool deviceCreated = false;

    void CreateInstance();
    void CreateSurface(GLFWwindow* window);
    void PickPhysicalDevice();
    void CreateLogicalDevice();
};
