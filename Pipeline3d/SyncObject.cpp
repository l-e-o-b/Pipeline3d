#include "SyncObject.h"

void SyncObjects::Create(vk::raii::Device& device, std::size_t imageCount)
{
    const vk::SemaphoreCreateInfo semaphoreInfo{};
    imageAvailable = vk::raii::Semaphore(device, semaphoreInfo);
    renderFinished.clear();
    renderFinished.reserve(imageCount);
    for (std::size_t i = 0; i < imageCount; ++i)
        renderFinished.emplace_back(device, semaphoreInfo);

    vk::FenceCreateInfo fenceInfo{};
    fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;
    inFlightFence = vk::raii::Fence(device, fenceInfo);
}

void SyncObjects::Cleanup()
{
    renderFinished.clear();
    imageAvailable = nullptr;
    inFlightFence = nullptr;
}
vk::raii::Semaphore& SyncObjects::GetImageAvailable() { return imageAvailable; }
vk::raii::Semaphore& SyncObjects::GetRenderFinished(std::size_t index) { return renderFinished.at(index); }
vk::raii::Fence& SyncObjects::GetInFlightFence() { return inFlightFence; }
