//
// Created by xtrit on 1/11/17.
//

#include "RenderPass.hpp"

using namespace vk;
using namespace std;


void vulkan::RenderPass::init() {
    initRenderPass();
    initSetLayouts();
    initPipelineLayout();
    initPipeline();
}

void vulkan::RenderPass::initRenderPass() {
    AttachmentDescription depthBuffer;
    depthBuffer.format = Format::eD32Sfloat;
    depthBuffer.loadOp = AttachmentLoadOp::eClear;
    depthBuffer.storeOp = AttachmentStoreOp::eDontCare;
    depthBuffer.stencilLoadOp = AttachmentLoadOp::eDontCare;
    depthBuffer.stencilLoadOp = AttachmentLoadOp::eDontCare;
    depthBuffer.initialLayout = ImageLayout::eUndefined;
    depthBuffer.finalLayout = ImageLayout::eDepthStencilAttachmentOptimal;

    AttachmentDescription colorBuffer;
    colorBuffer.format = Format::eB8G8R8A8Srgb;
    colorBuffer.loadOp = AttachmentLoadOp::eClear;
    colorBuffer.storeOp = AttachmentStoreOp::eStore;
    colorBuffer.stencilLoadOp = AttachmentLoadOp::eDontCare;
    colorBuffer.stencilLoadOp = AttachmentLoadOp::eDontCare;
    colorBuffer.initialLayout = ImageLayout::eUndefined;
    colorBuffer.finalLayout = ImageLayout::ePresentSrcKHR;

    vector<AttachmentDescription> attachments = {depthBuffer, colorBuffer};

    AttachmentReference depthAttachment;
    depthAttachment.attachment = 0;
    depthAttachment.layout = ImageLayout::eDepthStencilAttachmentOptimal;

    AttachmentReference colorAttachment;
    colorAttachment.attachment = 1;
    colorAttachment.layout = ImageLayout::eColorAttachmentOptimal;

    SubpassDescription subpass;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachment;
    subpass.pDepthStencilAttachment = &depthAttachment;

    vector<SubpassDescription> subpasses{subpass};
    vector<SubpassDependency> dependencies{};

    RenderPassCreateInfo renderPassInfo;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = static_cast<uint32_t>(subpasses.size());
    renderPassInfo.pSubpasses = subpasses.data();
    renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
    renderPassInfo.pDependencies = dependencies.data();

    opaqueRenderPass = device.createRenderPass(renderPassInfo);
}

void vulkan::RenderPass::initSetLayouts() {
    DescriptorSetLayoutBinding vertexSet0Binding;
    vertexSet0Binding.binding = 0;
    vertexSet0Binding.descriptorCount = 1;
    vertexSet0Binding.descriptorType = DescriptorType::eUniformBufferDynamic;
    vertexSet0Binding.stageFlags = ShaderStageFlagBits::eVertex;
    vertexSet0Binding.pImmutableSamplers = nullptr;

    vector<DescriptorSetLayoutBinding> set0Bindings = {vertexSet0Binding};

    DescriptorSetLayoutCreateInfo set0LayoutInfo;
    set0LayoutInfo.bindingCount = static_cast<uint32_t>(set0Bindings.size());
    set0LayoutInfo.pBindings = set0Bindings.data();

    set0Layout = device.createDescriptorSetLayout(set0LayoutInfo);


    DescriptorSetLayoutBinding vertexSet1Binding;
    vertexSet1Binding.binding = 0;
    vertexSet1Binding.descriptorCount = 1;
    vertexSet1Binding.descriptorType = DescriptorType::eUniformBuffer;
    vertexSet1Binding.stageFlags = ShaderStageFlagBits::eVertex;
    vertexSet1Binding.pImmutableSamplers = nullptr;

    vector<DescriptorSetLayoutBinding> set1Bindings = {vertexSet1Binding};

    DescriptorSetLayoutCreateInfo set1LayoutInfo;
    set1LayoutInfo.bindingCount = static_cast<uint32_t>(set1Bindings.size());
    set1LayoutInfo.pBindings = set1Bindings.data();

    set1Layout = device.createDescriptorSetLayout(set1LayoutInfo);
}

void vulkan::RenderPass::initPipelineLayout() {
    vector<DescriptorSetLayout> setLayouts{set0Layout, set1Layout};

    PipelineLayoutCreateInfo layoutInfo;
    layoutInfo.setLayoutCount = static_cast<uint32_t>(setLayouts.size());
    layoutInfo.pSetLayouts = setLayouts.data();
    layoutInfo.pushConstantRangeCount = 0;
    layoutInfo.pPushConstantRanges = nullptr;

    pipelineLayout = device.createPipelineLayout(layoutInfo);
}

void vulkan::RenderPass::initPipeline() {
    PipelineShaderStageCreateInfo vertexShaderStage;
    vertexShaderStage.stage = ShaderStageFlagBits::eVertex;
    vertexShaderStage.module = vertexShader;
    vertexShaderStage.pName = "main";

    PipelineShaderStageCreateInfo fragmentShaderStage;
    fragmentShaderStage.stage = ShaderStageFlagBits::eFragment;
    fragmentShaderStage.module = fragmentShader;
    fragmentShaderStage.pName = "main";

    vector<PipelineShaderStageCreateInfo> stages{vertexShaderStage, fragmentShaderStage};


    VertexInputBindingDescription vertexBinding;
    vertexBinding.binding = 0;
    vertexBinding.stride = 6 * sizeof(float);
    vertexBinding.inputRate = VertexInputRate::eVertex;

    VertexInputAttributeDescription vertexAttDescr;
    vertexAttDescr.binding = 0;
    vertexAttDescr.format = Format::eR32G32B32Sfloat;
    vertexAttDescr.location = 0;
    vertexAttDescr.offset = 0;

    VertexInputAttributeDescription normalAttDescr;
    normalAttDescr.binding = 0;
    normalAttDescr.format = Format::eR32G32B32Sfloat;
    normalAttDescr.location = 1;
    normalAttDescr.offset = 3 * sizeof(float);

    vector<VertexInputAttributeDescription> attributes{vertexAttDescr, normalAttDescr};

    PipelineVertexInputStateCreateInfo inputInfo;
    inputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributes.size());
    inputInfo.pVertexAttributeDescriptions = attributes.data();
    inputInfo.vertexBindingDescriptionCount = 1;
    inputInfo.pVertexBindingDescriptions = &vertexBinding;


    PipelineInputAssemblyStateCreateInfo assemblyInfo;
    assemblyInfo.topology = PrimitiveTopology::eTriangleList;
    assemblyInfo.primitiveRestartEnable = VK_FALSE;


    vector<Viewport> viewports{{0, 0, 800, 600, 0.0f, 1.0f}};
    vector<Rect2D> scissors{{{0, 0}, {800, 600}}};

    PipelineViewportStateCreateInfo viewportInfo;
    viewportInfo.viewportCount = static_cast<uint32_t>(viewports.size());
    viewportInfo.pViewports = viewports.data();
    viewportInfo.scissorCount = static_cast<uint32_t>(scissors.size());
    viewportInfo.pScissors = scissors.data();


    PipelineRasterizationStateCreateInfo rasterInfo;
    rasterInfo.depthClampEnable = VK_FALSE;
    rasterInfo.rasterizerDiscardEnable = VK_FALSE;
    rasterInfo.polygonMode = PolygonMode::eFill;
    rasterInfo.cullMode = CullModeFlagBits::eBack;
    rasterInfo.frontFace = FrontFace::eCounterClockwise;
    rasterInfo.depthBiasEnable = VK_FALSE;
    rasterInfo.depthBiasConstantFactor = 0.0f;
    rasterInfo.depthBiasClamp = 0.0f;
    rasterInfo.depthBiasSlopeFactor = 0.0f;
    rasterInfo.lineWidth = 1.0f;


    PipelineMultisampleStateCreateInfo multisampleInfo;

    PipelineDepthStencilStateCreateInfo depthInfo;
    depthInfo.depthTestEnable = VK_TRUE;
    depthInfo.depthWriteEnable = VK_TRUE;
    depthInfo.depthCompareOp = CompareOp::eLessOrEqual;
    depthInfo.depthBoundsTestEnable = VK_FALSE;
    depthInfo.stencilTestEnable = VK_FALSE;


    PipelineColorBlendAttachmentState blend;
    blend.blendEnable = VK_TRUE;
    blend.colorWriteMask = ColorComponentFlags()
                             | ColorComponentFlagBits::eR
                             | ColorComponentFlagBits::eG
                             | ColorComponentFlagBits::eB
                             | ColorComponentFlagBits::eA;
    blend.srcColorBlendFactor = BlendFactor::eSrcAlpha;
    blend.dstColorBlendFactor = BlendFactor::eOneMinusSrcAlpha;
    blend.colorBlendOp=BlendOp::eAdd;
    blend.srcAlphaBlendFactor = BlendFactor::eOne;
    blend.dstAlphaBlendFactor = BlendFactor::eZero;
    blend.alphaBlendOp = BlendOp::eAdd;
    vector<PipelineColorBlendAttachmentState> blendAttachments{blend};

    PipelineColorBlendStateCreateInfo blendInfo;
    blendInfo.logicOpEnable = VK_FALSE;
    blendInfo.attachmentCount = static_cast<uint32_t>(blendAttachments.size());
    blendInfo.pAttachments = blendAttachments.data();


    GraphicsPipelineCreateInfo pipelineCreateInfo;
    pipelineCreateInfo.stageCount = static_cast<uint32_t>(stages.size());
    pipelineCreateInfo.pStages = stages.data();
    pipelineCreateInfo.pVertexInputState = &inputInfo;
    pipelineCreateInfo.pInputAssemblyState = &assemblyInfo;
    pipelineCreateInfo.pTessellationState = {};
    pipelineCreateInfo.pViewportState = &viewportInfo;
    pipelineCreateInfo.pRasterizationState = &rasterInfo;
    pipelineCreateInfo.pMultisampleState = &multisampleInfo;
    pipelineCreateInfo.pDepthStencilState = &depthInfo;
    pipelineCreateInfo.pColorBlendState = &blendInfo;
    pipelineCreateInfo.pDynamicState = {};

    pipelineCreateInfo.layout = pipelineLayout;
    pipelineCreateInfo.renderPass = opaqueRenderPass;
    pipelineCreateInfo.subpass = 0;

    opaquePipeline = device.createGraphicsPipeline({}, pipelineCreateInfo);

}

vulkan::RenderPass::~RenderPass() {
    if (opaquePipeline) {
        device.destroyPipeline(opaquePipeline);
    }
    if (pipelineLayout) {
        device.destroyPipelineLayout(pipelineLayout);
    }
    if (set0Layout) {
        device.destroyDescriptorSetLayout(set0Layout);
    }
    if (set1Layout) {
        device.destroyDescriptorSetLayout(set1Layout);
    }
    if (opaqueRenderPass) {
        device.destroyRenderPass(opaqueRenderPass);
    }
}
