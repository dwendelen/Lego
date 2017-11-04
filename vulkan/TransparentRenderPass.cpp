//
// Created by xtrit on 4/11/17.
//

#include "TransparentRenderPass.hpp"


using namespace vk;
using namespace std;


void vulkan::TransparentRenderPass::init() {
    initRenderPass();
    initSetLayouts();
    initPipelineLayout();
    initPipelines();
}

void vulkan::TransparentRenderPass::initRenderPass() {
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
    subpass0.colorAttachmentCount = 0;
    subpass0.pColorAttachments = nullptr;
    subpass0.pDepthStencilAttachment = &depthAttachment;

    SubpassDescription subpass1;
    subpass1.colorAttachmentCount = 1;
    subpass1.pColorAttachments = &colorAttachment;
    subpass1.pDepthStencilAttachment = &depthAttachment;

    vector<SubpassDescription> subpasses{subpass0, subpass1};
    SubpassDependency subpassDependency;
    subpassDependency.srcSubpass = 0;
    subpassDependency.dstSubpass = 1;
    subpassDependency.srcStageMask = PipelineStageFlagBits::eEarlyFragmentTests;
    subpassDependency.dstStageMask = PipelineStageFlagBits::eEarlyFragmentTests;
    subpassDependency.srcAccessMask = AccessFlagBits::eDepthStencilAttachmentWrite;
    subpassDependency.srcAccessMask = AccessFlagBits::eDepthStencilAttachmentRead;
    vector<SubpassDependency> dependencies{subpassDependency};

    RenderPassCreateInfo renderPassInfo;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = static_cast<uint32_t>(subpasses.size());
    renderPassInfo.pSubpasses = subpasses.data();
    renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
    renderPassInfo.pDependencies = dependencies.data();

    opaqueRenderPass = device.createRenderPass(renderPassInfo);
}

void vulkan::TransparentRenderPass::initSetLayouts() {
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

void vulkan::TransparentRenderPass::initPipelineLayout() {
    vector<DescriptorSetLayout> setLayouts{set0Layout, set1Layout};

    PipelineLayoutCreateInfo layoutInfo;
    layoutInfo.setLayoutCount = static_cast<uint32_t>(setLayouts.size());
    layoutInfo.pSetLayouts = setLayouts.data();
    layoutInfo.pushConstantRangeCount = 0;
    layoutInfo.pPushConstantRanges = nullptr;

    pipelineLayout = device.createPipelineLayout(layoutInfo);
}

void vulkan::TransparentRenderPass::initPipelines() {
    PipelineShaderStageCreateInfo vertexShaderStage;
    vertexShaderStage.stage = ShaderStageFlagBits::eVertex;
    vertexShaderStage.module = vertexShader;
    vertexShaderStage.pName = "main";

    PipelineShaderStageCreateInfo fragmentShaderStage;
    fragmentShaderStage.stage = ShaderStageFlagBits::eFragment;
    fragmentShaderStage.module = fragmentShader;
    fragmentShaderStage.pName = "main";

    vector<PipelineShaderStageCreateInfo> stages0{vertexShaderStage};


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



    vector<PipelineColorBlendAttachmentState> blendAttachments0{};

    PipelineColorBlendStateCreateInfo blendInfo0;
    blendInfo0.logicOpEnable = VK_FALSE;
    blendInfo0.attachmentCount = static_cast<uint32_t>(blendAttachments0.size());
    blendInfo0.pAttachments = blendAttachments0.data();


    GraphicsPipelineCreateInfo pipelineCreateInfo0;
    pipelineCreateInfo0.stageCount = static_cast<uint32_t>(stages0.size());
    pipelineCreateInfo0.pStages = stages0.data();
    pipelineCreateInfo0.pVertexInputState = &inputInfo;
    pipelineCreateInfo0.pInputAssemblyState = &assemblyInfo;
    pipelineCreateInfo0.pTessellationState = {};
    pipelineCreateInfo0.pViewportState = &viewportInfo;
    pipelineCreateInfo0.pRasterizationState = &rasterInfo;
    pipelineCreateInfo0.pMultisampleState = &multisampleInfo;
    pipelineCreateInfo0.pDepthStencilState = &depthInfo;
    pipelineCreateInfo0.pColorBlendState = nullptr;//&blendInfo0;
    pipelineCreateInfo0.pDynamicState = {};

    pipelineCreateInfo0.layout = pipelineLayout;
    pipelineCreateInfo0.renderPass = opaqueRenderPass;
    pipelineCreateInfo0.subpass = 0;

    pipeline0 = device.createGraphicsPipeline({}, pipelineCreateInfo0);



    vector<PipelineShaderStageCreateInfo> stages1{vertexShaderStage, fragmentShaderStage};

    PipelineColorBlendAttachmentState blend1;
    blend1.blendEnable = VK_TRUE;
    blend1.colorWriteMask = ColorComponentFlags()
                            | ColorComponentFlagBits::eR
                            | ColorComponentFlagBits::eG
                            | ColorComponentFlagBits::eB
                            | ColorComponentFlagBits::eA;
    blend1.srcColorBlendFactor = BlendFactor::eSrcAlpha;
    blend1.dstColorBlendFactor = BlendFactor::eOneMinusSrcAlpha;
    blend1.colorBlendOp=BlendOp::eAdd;
    blend1.srcAlphaBlendFactor = BlendFactor::eOne;
    blend1.dstAlphaBlendFactor = BlendFactor::eZero;
    blend1.alphaBlendOp = BlendOp::eAdd;
    vector<PipelineColorBlendAttachmentState> blendAttachments1{blend1};

    PipelineColorBlendStateCreateInfo blendInfo1;
    blendInfo1.logicOpEnable = VK_FALSE;
    blendInfo1.attachmentCount = static_cast<uint32_t>(blendAttachments1.size());
    blendInfo1.pAttachments = blendAttachments1.data();


    GraphicsPipelineCreateInfo pipelineCreateInfo1;
    pipelineCreateInfo1.stageCount = static_cast<uint32_t>(stages1.size());
    pipelineCreateInfo1.pStages = stages1.data();
    pipelineCreateInfo1.pVertexInputState = &inputInfo;
    pipelineCreateInfo1.pInputAssemblyState = &assemblyInfo;
    pipelineCreateInfo1.pTessellationState = {};
    pipelineCreateInfo1.pViewportState = &viewportInfo;
    pipelineCreateInfo1.pRasterizationState = &rasterInfo;
    pipelineCreateInfo1.pMultisampleState = &multisampleInfo;
    pipelineCreateInfo1.pDepthStencilState = &depthInfo;
    pipelineCreateInfo1.pColorBlendState = &blendInfo1;
    pipelineCreateInfo1.pDynamicState = {};

    pipelineCreateInfo1.layout = pipelineLayout;
    pipelineCreateInfo1.renderPass = opaqueRenderPass;
    pipelineCreateInfo1.subpass = 1;

    pipeline1 = device.createGraphicsPipeline({}, pipelineCreateInfo1);

}

vulkan::TransparentRenderPass::~TransparentRenderPass() {
    if (pipeline0) {
        device.destroyPipeline(pipeline0);
    }
    if (pipeline1) {
        device.destroyPipeline(pipeline1);
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
