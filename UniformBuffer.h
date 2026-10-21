#pragma once

#include "Buffer.h"
#include <glm/glm.hpp>

struct UniformBufferObject
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
};

class UniformBuffer
{
public:
    void Create(
        vk::raii::PhysicalDevice& physicalDevice,
        vk::raii::Device& device);

    void Update(
        vk::raii::Device& device,
        const UniformBufferObject& ubo);

    void Cleanup();

    vk::raii::Buffer& Get();

private:
    Buffer buffer;
};