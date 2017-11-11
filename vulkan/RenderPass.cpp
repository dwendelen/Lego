//
// Created by xtrit on 4/11/17.
//

#include "RenderPass.hpp"


using namespace vk;
using namespace std;


void vulkan::RenderPass::init() {
    initRenderPass();
    initSetLayouts();
    initPipelineLayout();
    initPipelines();
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


    SubpassDescription subpass0;
    subpass0.colorAttachmentCount = 1;
    subpass0.pColorAttachments = &colorAttachment;
    subpass0.pDepthStencilAttachment = &depthAttachment;
    
    SubpassDescription subpass1;
    subpass1.colorAttachmentCount = 0;
    subpass1.pColorAttachments = nullptr;
    subpass1.pDepthStencilAttachment = &depthAttachment;

    SubpassDescription subpass2;
    subpass2.colorAttachmentCount = 1;
    subpass2.pColorAttachments = &colorAttachment;
    subpass2.pDepthStencilAttachment = &depthAttachment;

    vector<SubpassDescription> subpasses{subpass0, subpass1, subpass2};
    SubpassDependency dependency0to1;
    dependency0to1.srcSubpass = 0;
    dependency0to1.dstSubpass = 1;
    dependency0to1.srcStageMask = PipelineStageFlagBits::eEarlyFragmentTests;
    dependency0to1.dstStageMask = PipelineStageFlagBits::eEarlyFragmentTests;
    dependency0to1.srcAccessMask = AccessFlagBits::eDepthStencilAttachmentWrite;
    dependency0to1.srcAccessMask = AccessFlagBits::eDepthStencilAttachmentRead;

    SubpassDependency dependency1to2;
    dependency1to2.srcSubpass = 1;
    dependency1to2.dstSubpass = 2;
    dependency1to2.srcStageMask = PipelineStageFlagBits::eEarlyFragmentTests;
    dependency1to2.dstStageMask = PipelineStageFlagBits::eEarlyFragmentTests;
    dependency1to2.srcAccessMask = AccessFlagBits::eDepthStencilAttachmentWrite;
    dependency1to2.srcAccessMask = AccessFlagBits::eDepthStencilAttachmentRead;

    SubpassDependency dependency0to2;
    dependency0to2.srcSubpass = 0;
    dependency0to2.dstSubpass = 2;
    dependency0to2.srcStageMask = PipelineStageFlagBits::eColorAttachmentOutput;
    dependency0to2.dstStageMask = PipelineStageFlagBits::eColorAttachmentOutput;
    dependency0to2.srcAccessMask = AccessFlagBits::eColorAttachmentWrite;
    dependency0to2.srcAccessMask = AccessFlagBits::eColorAttachmentRead;
    
    vector<SubpassDependency> dependencies{dependency0to1, dependency1to2, dependency0to2};
    
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

void vulkan::RenderPass::initPipelines() {
    PipelineShaderStageCreateInfo vertexShaderStage;
    vertexShaderStage.stage = ShaderStageFlagBits::eVertex;
    vertexShaderStage.module = vertexShader;
    vertexShaderStage.pName = "main";

    PipelineShaderStageCreateInfo fragmentShaderStage;
    fragmentShaderStage.stage = ShaderStageFlagBits::eFragment;
    fragmentShaderStage.module = fragmentShader;
    fragmentShaderStage.pName = "main";

    vector<PipelineShaderStageCreateInfo> stagesWithoutColorOutput{vertexShaderStage};
    vector<PipelineShaderStageCreateInfo> stagesWithColorOutput{vertexShaderStage, fragmentShaderStage};


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


    PipelineColorBlendAttachmentState blendColorAttachment;
    blendColorAttachment.blendEnable = VK_TRUE;
    blendColorAttachment.colorWriteMask = ColorComponentFlags()
                                          | ColorComponentFlagBits::eR
                                          | ColorComponentFlagBits::eG
                                          | ColorComponentFlagBits::eB
                                          | ColorComponentFlagBits::eA;
    blendColorAttachment.srcColorBlendFactor = BlendFactor::eSrcAlpha;
    blendColorAttachment.dstColorBlendFactor = BlendFactor::eOneMinusSrcAlpha;
    blendColorAttachment.colorBlendOp=BlendOp::eAdd;
    blendColorAttachment.srcAlphaBlendFactor = BlendFactor::eOne;
    blendColorAttachment.dstAlphaBlendFactor = BlendFactor::eZero;
    blendColorAttachment.alphaBlendOp = BlendOp::eAdd;
    vector<PipelineColorBlendAttachmentState> blendColorAttachments{blendColorAttachment};

    PipelineColorBlendStateCreateInfo blendInfoWithColorOutput;
    blendInfoWithColorOutput.logicOpEnable = VK_FALSE;
    blendInfoWithColorOutput.attachmentCount = static_cast<uint32_t>(blendColorAttachments.size());
    blendInfoWithColorOutput.pAttachments = blendColorAttachments.data();



    GraphicsPipelineCreateInfo pipelineCreateInfo0;
    pipelineCreateInfo0.stageCount = static_cast<uint32_t>(stagesWithColorOutput.size());
    pipelineCreateInfo0.pStages = stagesWithColorOutput.data();
    pipelineCreateInfo0.pVertexInputState = &inputInfo;
    pipelineCreateInfo0.pInputAssemblyState = &assemblyInfo;
    pipelineCreateInfo0.pTessellationState = {};
    pipelineCreateInfo0.pViewportState = &viewportInfo;
    pipelineCreateInfo0.pRasterizationState = &rasterInfo;
    pipelineCreateInfo0.pMultisampleState = &multisampleInfo;
    pipelineCreateInfo0.pDepthStencilState = &depthInfo;
    pipelineCreateInfo0.pColorBlendState = &blendInfoWithColorOutput;
    pipelineCreateInfo0.pDynamicState = {};

    pipelineCreateInfo0.layout = pipelineLayout;
    pipelineCreateInfo0.renderPass = opaqueRenderPass;
    pipelineCreateInfo0.subpass = 0;

    pipeline0 = device.createGraphicsPipeline({}, pipelineCreateInfo0);
    
    
    GraphicsPipelineCreateInfo pipelineCreateInfo1;
    pipelineCreateInfo1.stageCount = static_cast<uint32_t>(stagesWithoutColorOutput.size());
    pipelineCreateInfo1.pStages = stagesWithoutColorOutput.data();
    pipelineCreateInfo1.pVertexInputState = &inputInfo;
    pipelineCreateInfo1.pInputAssemblyState = &assemblyInfo;
    pipelineCreateInfo1.pTessellationState = {};
    pipelineCreateInfo1.pViewportState = &viewportInfo;
    pipelineCreateInfo1.pRasterizationState = &rasterInfo;
    pipelineCreateInfo1.pMultisampleState = &multisampleInfo;
    pipelineCreateInfo1.pDepthStencilState = &depthInfo;
    pipelineCreateInfo1.pColorBlendState = nullptr;
    pipelineCreateInfo1.pDynamicState = {};

    pipelineCreateInfo1.layout = pipelineLayout;
    pipelineCreateInfo1.renderPass = opaqueRenderPass;
    pipelineCreateInfo1.subpass = 1;

    pipeline1 = device.createGraphicsPipeline({}, pipelineCreateInfo1);
    

    GraphicsPipelineCreateInfo pipelineCreateInfo2;
    pipelineCreateInfo2.stageCount = static_cast<uint32_t>(stagesWithColorOutput.size());
    pipelineCreateInfo2.pStages = stagesWithColorOutput.data();
    pipelineCreateInfo2.pVertexInputState = &inputInfo;
    pipelineCreateInfo2.pInputAssemblyState = &assemblyInfo;
    pipelineCreateInfo2.pTessellationState = {};
    pipelineCreateInfo2.pViewportState = &viewportInfo;
    pipelineCreateInfo2.pRasterizationState = &rasterInfo;
    pipelineCreateInfo2.pMultisampleState = &multisampleInfo;
    pipelineCreateInfo2.pDepthStencilState = &depthInfo;
    pipelineCreateInfo2.pColorBlendState = &blendInfoWithColorOutput;
    pipelineCreateInfo2.pDynamicState = {};

    pipelineCreateInfo2.layout = pipelineLayout;
    pipelineCreateInfo2.renderPass = opaqueRenderPass;
    pipelineCreateInfo2.subpass = 2;

    pipeline2 = device.createGraphicsPipeline({}, pipelineCreateInfo2);

}

vulkan::RenderPass::~RenderPass() {
    if (pipeline0) {
        device.destroyPipeline(pipeline0);
    }
    if (pipeline1) {
        device.destroyPipeline(pipeline1);
    }
    if (pipeline2) {
        device.destroyPipeline(pipeline2);
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
