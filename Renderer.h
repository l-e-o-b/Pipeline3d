#pragma once

#include "VulkanContext.h"
#include "Swapchain.h"
#include "RenderPass.h"
#include "GraphicsPipeline.h"
#include "Framebuffer.h"
#include "CommandManager.h"
#include "SyncObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"
#include "DescriptorManager.h"

class Renderer
{
public:
    void Init(GLFWwindow* window);
    void DrawFrame();
    void Cleanup();

private:
    VulkanContext context;
    Swapchain swapchain;
    RenderPass renderPass;
    GraphicsPipeline graphicsPipeline;
    FramebufferManager framebufferManager;
    CommandManager commandManager;
    SyncObjects syncObjects;
    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;
    UniformBuffer uniformBuffer;
    DescriptorManager descriptorManager;
};
