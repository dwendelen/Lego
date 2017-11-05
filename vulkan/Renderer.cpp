//
// Created by xtrit on 21/10/17.
//

#include "Renderer.hpp"
#include "MemoryManager.hpp"
#include "../engine/Scene.hpp"
#include "SimpleRenderPass.hpp"

#include <iostream>
#include <fstream>
#include <SDL_syswm.h>

#ifdef __LINUX__

#include <X11/Xlib-xcb.h>
#include <chrono>

#endif

using namespace vulkan;
using namespace vk;
using namespace std;


template<typename T>
bool hasFlags(T actual, T expected) {
    return (actual & expected) == expected;
}


void vulkan::Renderer::init() {

    vector<char const *> instanceExtensions{
#ifdef VK_USE_PLATFORM_XLIB_KHR
            VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
#endif
#ifdef VK_USE_PLATFORM_XCB_KHR
            VK_KHR_XCB_SURFACE_EXTENSION_NAME,
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
            VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
            VK_KHR_SURFACE_EXTENSION_NAME,
            VK_EXT_DEBUG_REPORT_EXTENSION_NAME
    };
    vector<char const *> layers{"VK_LAYER_LUNARG_standard_validation"};
    vector<char const *> deviceExtensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    context = unique_ptr<Context>{new Context(layers, instanceExtensions, deviceExtensions)};
    context->load();

    Instance instance = context->getInstance();
    PhysicalDevice physical = context->getPhysical();
    Device device = context->getDevice();
    uint32_t queueFamilyIndex = context->getQueueFamilyIndex();


    queue = device.getQueue(queueFamilyIndex, 0);

    CommandPoolCreateInfo commandPoolCreateInfo;
    commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;
    commandPool = device.createCommandPool(commandPoolCreateInfo);

    vertexShader = unique_ptr<Shader>{new Shader("shaders/vert.spv", device)};
    fragmentShader = unique_ptr<Shader>{new Shader("shaders/frag.spv", device)};

    vertexShader->load();
    fragmentShader->load();

    createOpagePipeline();

    memoryManager = unique_ptr<MemoryManager>(new MemoryManager(device, physical));
    memoryManager->init();


    display = unique_ptr<Display>(new Display(instance, physical, device, queue));
    display->init();
    cout << "TODO CHECK CAPABILITIES" << endl;



    ImageCreateInfo depthInfo;
    depthInfo.imageType = ImageType::e2D;
    depthInfo.format = Format::eD32Sfloat;
    depthInfo.extent.width = 800;
    depthInfo.extent.height = 600;
    depthInfo.extent.depth = 1;
    depthInfo.mipLevels = 1;
    depthInfo.arrayLayers = 1;
    depthInfo.samples = SampleCountFlagBits::e1;
    depthInfo.tiling = ImageTiling::eOptimal;
    depthInfo.usage = ImageUsageFlagBits::eDepthStencilAttachment;
    depthInfo.sharingMode = SharingMode::eExclusive;
    depthInfo.queueFamilyIndexCount = 0;
    depthInfo.pQueueFamilyIndices = nullptr;
    depthInfo.initialLayout = ImageLayout::eUndefined;

    depthBuffer = device.createImage(depthInfo);

    vector<Image> imagesToAlloc;
    imagesToAlloc.push_back(depthBuffer);

    memoryManager->allocateStatic(imagesToAlloc);

    ImageViewCreateInfo depthimageViewCreateInfo;
    depthimageViewCreateInfo.image = depthBuffer;
    depthimageViewCreateInfo.viewType = ImageViewType::e2D;
    depthimageViewCreateInfo.format = Format::eD32Sfloat;
    depthimageViewCreateInfo.components = {ComponentSwizzle::eIdentity, ComponentSwizzle::eIdentity,
                                           ComponentSwizzle::eIdentity, ComponentSwizzle::eIdentity};
    depthimageViewCreateInfo.subresourceRange.aspectMask = ImageAspectFlagBits::eDepth;
    depthimageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    depthimageViewCreateInfo.subresourceRange.levelCount = 1;
    depthimageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    depthimageViewCreateInfo.subresourceRange.layerCount = 1;

    depthView = device.createImageView(depthimageViewCreateInfo);

    for (auto image: display->getImages()) {
        ImageViewCreateInfo imageViewCreateInfo;
        imageViewCreateInfo.image = image;
        imageViewCreateInfo.viewType = ImageViewType::e2D;
        imageViewCreateInfo.format = Format::eB8G8R8A8Srgb;
        imageViewCreateInfo.components = {ComponentSwizzle::eIdentity, ComponentSwizzle::eIdentity,
                                          ComponentSwizzle::eIdentity, ComponentSwizzle::eIdentity};
        imageViewCreateInfo.subresourceRange.aspectMask = ImageAspectFlagBits::eColor;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;

        ImageView imageView = device.createImageView(imageViewCreateInfo);
        this->imageViews.push_back(imageView);

        vector<ImageView> attachments{depthView, imageView};

        FramebufferCreateInfo framebufferCreateInfo;
        framebufferCreateInfo.renderPass = transparentRenderPass->getRenderPass();
        framebufferCreateInfo.attachmentCount = attachments.size();
        framebufferCreateInfo.pAttachments = attachments.data();
        framebufferCreateInfo.width = 800;
        framebufferCreateInfo.height = 600;
        framebufferCreateInfo.layers = 1;

        Framebuffer framebuffer = device.createFramebuffer(framebufferCreateInfo);
        this->frameBuffers.push_back(framebuffer);
    }

    frameBufferReady = device.createSemaphore({});
    renderingDone = device.createSemaphore({});
    renderingDoneFence = device.createFence({});

    vector<DescriptorPoolSize> descriptorPoolSizes{
            {DescriptorType::eUniformBufferDynamic, 1},
            {DescriptorType::eUniformBuffer,        1},
    };

    DescriptorPoolCreateInfo descriptorPoolCreateInfo;
    descriptorPoolCreateInfo.maxSets = 2;
    descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size());
    descriptorPoolCreateInfo.pPoolSizes = descriptorPoolSizes.data();

    this->descriptorPool = device.createDescriptorPool(descriptorPoolCreateInfo);

    return;
}


void Renderer::createOpagePipeline() {
    Device device = context->getDevice();

    transparentRenderPass = unique_ptr<TransparentRenderPass>(
            new TransparentRenderPass(
                    device,
                    vertexShader->getShaderModule(),
                    fragmentShader->getShaderModule()
            )
    );
    transparentRenderPass->init();
}

void vulkan::Renderer::render(engine::Scene<vulkan::ModelData, vulkan::ObjectData> &scene) {
    Device device = context->getDevice();

    uint32_t imageIdx = display->acquireNextFrameBufferId(frameBufferReady);

    device.resetCommandPool(commandPool, {});
    device.resetDescriptorPool(descriptorPool);

    CommandBufferAllocateInfo commandBufferInfo;
    commandBufferInfo.commandPool = commandPool;
    commandBufferInfo.commandBufferCount = 1;
    commandBufferInfo.level = CommandBufferLevel::ePrimary;

    CommandBuffer commandBuffer = device.allocateCommandBuffers(commandBufferInfo)[0];

    vector<DescriptorSetLayout> descriptorSetLayouts {transparentRenderPass->getSetLayout0(), transparentRenderPass->getSetLayout1()};

    DescriptorSetAllocateInfo descriptorSetAllocateInfo;
    descriptorSetAllocateInfo.descriptorPool = descriptorPool;
    descriptorSetAllocateInfo.descriptorSetCount = static_cast<uint32_t>(descriptorSetLayouts.size());
    descriptorSetAllocateInfo.pSetLayouts = descriptorSetLayouts.data();

    vector<DescriptorSet> descriptorSets = device.allocateDescriptorSets(descriptorSetAllocateInfo);

    CommandBufferBeginInfo commandBufferBeginInfo;
    commandBufferBeginInfo.flags = CommandBufferUsageFlagBits::eOneTimeSubmit;
    commandBuffer.begin(commandBufferBeginInfo);

    DescriptorBufferInfo bufferInfo0;
    bufferInfo0.buffer = memoryManager->getObjectBuffer();
    bufferInfo0.offset = 0;
    bufferInfo0.range = VK_WHOLE_SIZE;

    WriteDescriptorSet writeDescriptor00;
    writeDescriptor00.dstSet = descriptorSets[0];
    writeDescriptor00.dstBinding = 0;
    writeDescriptor00.dstArrayElement = 0;
    writeDescriptor00.descriptorCount = 1;
    writeDescriptor00.descriptorType = DescriptorType::eUniformBufferDynamic;
    writeDescriptor00.pBufferInfo = &bufferInfo0;


    DescriptorBufferInfo bufferInfo1;
    bufferInfo1.buffer = memoryManager->getPvBuffer();
    bufferInfo1.offset = 0;
    bufferInfo1.range = VK_WHOLE_SIZE;

    WriteDescriptorSet writeDescriptor10;
    writeDescriptor10.dstSet = descriptorSets[1];
    writeDescriptor10.dstBinding = 0;
    writeDescriptor10.dstArrayElement = 0;
    writeDescriptor10.descriptorCount = 1;
    writeDescriptor10.descriptorType = DescriptorType::eUniformBuffer;
    writeDescriptor10.pBufferInfo = &bufferInfo1;

    device.updateDescriptorSets(
            vector<WriteDescriptorSet>{
                    writeDescriptor00,
                    writeDescriptor10
            },
            {}
    );


    ClearValue clearValues[2];
    clearValues[0].depthStencil = ClearDepthStencilValue{1.0f, 0};
    clearValues[1].color = ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.4f, 1.0f});


    RenderPassBeginInfo renderPassBeginInfo;
    renderPassBeginInfo.renderPass = transparentRenderPass->getRenderPass();
    renderPassBeginInfo.clearValueCount = 2;
    renderPassBeginInfo.pClearValues = clearValues;
    renderPassBeginInfo.renderArea = Rect2D{{0,   0},
                                            {800, 600}};
    renderPassBeginInfo.framebuffer = frameBuffers[imageIdx];


    commandBuffer.beginRenderPass(renderPassBeginInfo, SubpassContents::eInline);
    commandBuffer.bindDescriptorSets(PipelineBindPoint::eGraphics, transparentRenderPass->getPipelineLayout(), 1,
                                     {descriptorSets[1]}, {});

    Buffer modelBuffer = memoryManager->getModelBuffer();

    commandBuffer.bindPipeline(PipelineBindPoint::eGraphics, transparentRenderPass->getPipeline0());

    for (auto object: scene.objects) {
        DeviceSize offsetIndices = object.getModel().getRenderData().indexOffset;
        DeviceSize offsetVertices = object.getModel().getRenderData().vertexOffset;
        DeviceSize offsetUniform = object.objectData.dataOffset;

        commandBuffer.bindDescriptorSets(PipelineBindPoint::eGraphics, transparentRenderPass->getPipelineLayout(), 0,
                                         {descriptorSets[0]}, {static_cast<uint32_t>(offsetUniform)});
        commandBuffer.bindVertexBuffers(0, 1, &modelBuffer, &offsetVertices);
        commandBuffer.bindIndexBuffer(modelBuffer, offsetIndices, IndexType::eUint32);

        uint32_t nbOfIndices = static_cast<uint32_t>(object.getModel().getIndices().size() * 3);
        commandBuffer.drawIndexed(nbOfIndices, 1, 0, 0, 0);
    }

    commandBuffer.nextSubpass(SubpassContents::eInline);
    commandBuffer.bindPipeline(PipelineBindPoint::eGraphics, transparentRenderPass->getPipeline1());

    auto object = scene.controllingObject;
    DeviceSize offsetIndices = object.getModel().getRenderData().indexOffset;
    DeviceSize offsetVertices = object.getModel().getRenderData().vertexOffset;
    DeviceSize offsetUniform = object.objectData.dataOffset;
    uint32_t nbOfIndices = static_cast<uint32_t>(object.getModel().getIndices().size() * 3);

    commandBuffer.bindDescriptorSets(PipelineBindPoint::eGraphics, transparentRenderPass->getPipelineLayout(), 0,
                                     {descriptorSets[0]}, {static_cast<uint32_t>(offsetUniform)});
    commandBuffer.bindVertexBuffers(0, 1, &modelBuffer, &offsetVertices);
    commandBuffer.bindIndexBuffer(modelBuffer, offsetIndices, IndexType::eUint32);
    commandBuffer.drawIndexed(nbOfIndices, 1, 0, 0, 0);

    commandBuffer.nextSubpass(SubpassContents::eInline);
    commandBuffer.bindPipeline(PipelineBindPoint::eGraphics, transparentRenderPass->getPipeline2());
    commandBuffer.drawIndexed(nbOfIndices, 1, 0, 0, 0);


    commandBuffer.endRenderPass();
    commandBuffer.end();

    PipelineStageFlags waitStage = PipelineStageFlagBits::eTopOfPipe;

    SubmitInfo submitInfo;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &frameBufferReady;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &renderingDone;
    submitInfo.pWaitDstStageMask = &waitStage;


    device.resetFences(renderingDoneFence);
    queue.submit(submitInfo, renderingDoneFence);

    display->display(renderingDone, imageIdx);

    device.waitForFences({renderingDoneFence}, VK_TRUE, 10000000000);
}

vulkan::Renderer::~Renderer() {
    Device device = context->getDevice();

    if (device) {
        device.waitIdle();
    }
    for (auto fb: frameBuffers) {
        device.destroyFramebuffer(fb);
    }

    for (auto iv: imageViews) {
        device.destroyImageView(iv);
    }

    if (depthView) {
        device.destroyImageView(depthView);
    }
    if(depthBuffer) {
        device.destroyImage(depthBuffer);
    }

    if (descriptorPool) {
        device.destroyDescriptorPool(descriptorPool);
    }

    transparentRenderPass.reset();
    vertexShader.reset();
    fragmentShader.reset();

    if (commandPool) {
        device.destroyCommandPool(commandPool);
    }

    if(renderingDone) {
        device.destroySemaphore(renderingDone);
    }
    if(frameBufferReady) {
        device.destroySemaphore(frameBufferReady);
    }
    if(renderingDoneFence) {
        device.destroyFence(renderingDoneFence);
    }

    display.reset();
    memoryManager.reset();
    context.reset();
}

void Renderer::loadModel(engine::Model<ModelData> &model) {
    memoryManager->loadModel(model);
}

void Renderer::loadObject(engine::Object<ModelData, ObjectData> &object) {
    memoryManager->loadObject(object);
}



