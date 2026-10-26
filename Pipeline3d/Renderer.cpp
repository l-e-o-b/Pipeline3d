#include "Renderer.h"
#include <array>
#include <limits>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Renderer::Init(GLFWwindow* window)
{
    context.Create(window);

    vertexBuffer.Create(
        context.GetPhysicalDevice(),
        context.GetDevice());

    indexBuffer.Create(
        context.GetPhysicalDevice(),
        context.GetDevice());

    uniformBuffer.Create(
        context.GetPhysicalDevice(),
        context.GetDevice()
    );

    descriptorManager.Create(
        context.GetDevice(),
        uniformBuffer
    );

    UniformBufferObject ubo{};

    ubo.model = glm::mat4(1.0f);

    ubo.view = glm::lookAt(
        glm::vec3(2.0f, 2.0f, 2.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    ubo.projection = glm::perspective(
        glm::radians(45.0f),
        1920.0f / 1080.0f,
        0.1f,
        10.0f
    );

    ubo.projection[1][1] *= -1.0f;

    uniformBuffer.Update(
        context.GetDevice(),
        ubo
    );

    swapchain.Create(
        context.GetPhysicalDevice(), 
        context.GetDevice(),
        context.GetSurface(),
        window, context.GetGraphicsQueueFamilyIndex(),
        context.GetPresentQueueFamilyIndex());

    renderPass.Create(
        context.GetDevice(),
        swapchain.GetImageFormat());

    graphicsPipeline.Create(
        context.GetDevice(),
        renderPass.Get(),
        swapchain.GetExtent(),
        descriptorManager.GetLayout()
    );

    framebufferManager.Create(
        context.GetDevice(),
        renderPass.Get(),
        swapchain.GetImageViews(),
        swapchain.GetExtent());

    commandManager.Create(
        context.GetDevice(),
        context.GetGraphicsQueueFamilyIndex(),
        framebufferManager.GetAll(),
        renderPass.Get(),
        graphicsPipeline.Get(),
        graphicsPipeline.GetLayout(),
        swapchain.GetExtent(),
        vertexBuffer,
        indexBuffer,
        descriptorManager.Get()
    );

    syncObjects.Create(context.GetDevice(),
        swapchain.GetImages().size());
}

void Renderer::DrawFrame()
{
    auto& device = context.GetDevice();
    auto& fence = syncObjects.GetInFlightFence();
    device.waitForFences({ *fence }, VK_TRUE, UINT64_MAX);

    const auto acquired = swapchain.GetHandle().acquireNextImage(
        UINT64_MAX, *syncObjects.GetImageAvailable());
    const uint32_t imageIndex = acquired.value;
    device.resetFences({ *fence });

    const vk::Semaphore waitSemaphore = *syncObjects.GetImageAvailable();
    const vk::Semaphore signalSemaphore = *syncObjects.GetRenderFinished(imageIndex);
    const vk::CommandBuffer commandBuffer = *commandManager.GetAll().at(imageIndex);
    const vk::PipelineStageFlags waitStage =
        vk::PipelineStageFlagBits::eColorAttachmentOutput;

    vk::SubmitInfo submit{};
    submit.waitSemaphoreCount = 1;
    submit.pWaitSemaphores = &waitSemaphore;
    submit.pWaitDstStageMask = &waitStage;
    submit.commandBufferCount = 1;
    submit.pCommandBuffers = &commandBuffer;
    submit.signalSemaphoreCount = 1;
    submit.pSignalSemaphores = &signalSemaphore;
    context.GetGraphicsQueue().submit(submit, *fence);

    const vk::SwapchainKHR rawSwapchain = *swapchain.GetHandle();
    vk::PresentInfoKHR present{};
    present.waitSemaphoreCount = 1;
    present.pWaitSemaphores = &signalSemaphore;
    present.swapchainCount = 1;
    present.pSwapchains = &rawSwapchain;
    present.pImageIndices = &imageIndex;
    context.GetPresentQueue().presentKHR(present);
}

void Renderer::Cleanup()
{
    if (context.HasDevice())
        context.GetDevice().waitIdle();

    syncObjects.Cleanup();

    descriptorManager.Cleanup();

    commandManager.Cleanup();

    indexBuffer.Cleanup();
    vertexBuffer.Cleanup();

    uniformBuffer.Cleanup();

    framebufferManager.Cleanup();

    graphicsPipeline.Cleanup();

    renderPass.Cleanup();

    swapchain.Cleanup();

    context.Cleanup();
}
