#include "GraphicsPipeline.h"

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
    std::vector<uint32_t> ReadSpirv(const char* filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);
        if (!file.is_open()) throw std::runtime_error(std::string("Shader introuvable : ") + filename);
        const auto byteSize = static_cast<size_t>(file.tellg());
        if (byteSize == 0 || byteSize % sizeof(uint32_t) != 0)
            throw std::runtime_error(std::string("Fichier SPIR-V invalide : ") + filename);
        std::vector<uint32_t> code(byteSize / sizeof(uint32_t));
        file.seekg(0);
        file.read(reinterpret_cast<char*>(code.data()), static_cast<std::streamsize>(byteSize));
        if (!file) throw std::runtime_error(std::string("Erreur de lecture : ") + filename);
        return code;
    }
}

void GraphicsPipeline::Create(vk::raii::Device& device,
    vk::raii::RenderPass& renderPass, vk::Extent2D extent, vk::raii::DescriptorSetLayout& descriptorSetLayout)
{
    const auto vertexCode = ReadSpirv("triangle.vert.spv");
    const auto fragmentCode = ReadSpirv("triangle.frag.spv");
    vk::ShaderModuleCreateInfo vertexInfo{};
    vertexInfo.codeSize = vertexCode.size() * sizeof(uint32_t);
    vertexInfo.pCode = vertexCode.data();
    vk::ShaderModuleCreateInfo fragmentInfo{};
    fragmentInfo.codeSize = fragmentCode.size() * sizeof(uint32_t);
    fragmentInfo.pCode = fragmentCode.data();
    vk::raii::ShaderModule vertexShader(device, vertexInfo);
    vk::raii::ShaderModule fragmentShader(device, fragmentInfo);

    vk::PipelineShaderStageCreateInfo stages[2]{};
    stages[0].stage = vk::ShaderStageFlagBits::eVertex;
    stages[0].module = *vertexShader;
    stages[0].pName = "main";
    stages[1].stage = vk::ShaderStageFlagBits::eFragment;
    stages[1].module = *fragmentShader;
    stages[1].pName = "main";

    vk::VertexInputBindingDescription binding{};
    binding.binding = 0;
    binding.stride = sizeof(Vertex);
    binding.inputRate = vk::VertexInputRate::eVertex;

    vk::VertexInputAttributeDescription attributes[1]{};
    attributes[0].location = 0;
    attributes[0].binding = 0;
    attributes[0].format = vk::Format::eR32G32B32Sfloat;
    attributes[0].offset = offsetof(Vertex, x);

    vk::PipelineVertexInputStateCreateInfo vertexInput{};
    vertexInput.vertexBindingDescriptionCount = 1;
    vertexInput.pVertexBindingDescriptions = &binding;
    vertexInput.vertexAttributeDescriptionCount = 1;
    vertexInput.pVertexAttributeDescriptions = attributes;

    vk::PipelineInputAssemblyStateCreateInfo assembly{};
    assembly.topology = vk::PrimitiveTopology::eTriangleList;
    vk::Viewport viewport{ 0.0f, 0.0f, static_cast<float>(extent.width),
                           static_cast<float>(extent.height), 0.0f, 1.0f };
    vk::Rect2D scissor{ vk::Offset2D{ 0, 0 }, extent };
    vk::PipelineViewportStateCreateInfo viewportState{};
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    vk::PipelineRasterizationStateCreateInfo raster{};
    raster.polygonMode = vk::PolygonMode::eFill;
    raster.cullMode = vk::CullModeFlagBits::eNone;
    raster.frontFace = vk::FrontFace::eCounterClockwise;
    raster.lineWidth = 1.0f;
    vk::PipelineMultisampleStateCreateInfo multisample{};
    multisample.rasterizationSamples = vk::SampleCountFlagBits::e1;
    vk::PipelineColorBlendAttachmentState blendAttachment{};
    blendAttachment.blendEnable = VK_FALSE;
    blendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR |
        vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB |
        vk::ColorComponentFlagBits::eA;
    vk::PipelineColorBlendStateCreateInfo blend{};
    blend.attachmentCount = 1;
    blend.pAttachments = &blendAttachment;

    vk::PipelineLayoutCreateInfo layoutInfo{};
    layoutInfo.setLayoutCount = 1;
    layoutInfo.pSetLayouts = &*descriptorSetLayout;
    layout = vk::raii::PipelineLayout(
        device,
        layoutInfo
    );

    vk::GraphicsPipelineCreateInfo info{};
    info.stageCount = 2;
    info.pStages = stages;
    info.pVertexInputState = &vertexInput;
    info.pInputAssemblyState = &assembly;
    info.pViewportState = &viewportState;
    info.pRasterizationState = &raster;
    info.pMultisampleState = &multisample;
    info.pColorBlendState = &blend;
    info.layout = *layout;
    info.renderPass = *renderPass;
    info.subpass = 0;
    pipeline = vk::raii::Pipeline(device, nullptr, info);
}

void GraphicsPipeline::Cleanup()
{
    pipeline = nullptr;
    layout = nullptr;
}
vk::raii::Pipeline& GraphicsPipeline::Get() { return pipeline; }
vk::raii::PipelineLayout& GraphicsPipeline::GetLayout(){return layout;}