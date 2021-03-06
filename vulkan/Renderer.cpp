//
// Created by xtrit on 21/10/17.
//

#include "Renderer.hpp"
#include "ModelData.hpp"

#include "../engine/Model.hpp"
#include "../engine/Object.hpp"
#include "../engine/Events.hpp"
#include "../engine/EventBus.hpp"

#include <iostream>
#include <fstream>

#ifdef __LINUX__

#include <chrono>
#include <SDL_timer.h>

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
            VK_KHR_SURFACE_EXTENSION_NAME
    };
    vector<char const *> layers {};
    vector<char const *> deviceExtensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    if(debug) {
        instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        layers.push_back("VK_LAYER_LUNARG_standard_validation");
    }

    context = unique_ptr<Context>{new Context(layers, instanceExtensions, deviceExtensions, debug)};
    context->load();

    Instance instance = context->getInstance();
    PhysicalDevice physical = context->getPhysical();
    Device device = context->getDevice();
    uint32_t queueFamilyIndex = context->getQueueFamilyIndex();


    queue = device.getQueue(queueFamilyIndex, 0);



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



    vector<DescriptorSetLayout> descriptorSetLayouts {transparentRenderPass->getSetLayout0(), transparentRenderPass->getSetLayout1()};


    frontFrame = unique_ptr<Frame>(new Frame(device));
    backFrame = unique_ptr<Frame>(new Frame(device));

    frontFrame->init(queueFamilyIndex, descriptorSetLayouts);
    backFrame->init(queueFamilyIndex, descriptorSetLayouts);
}


void Renderer::createOpagePipeline() {
    Device device = context->getDevice();

    transparentRenderPass = unique_ptr<RenderPass>(
            new RenderPass(
                    device,
                    vertexShader->getShaderModule(),
                    fragmentShader->getShaderModule()
            )
    );
    transparentRenderPass->init();
}

void vulkan::Renderer::render() {
    backFrame->waitForFrameAndResetResources();
    
    bool readmore = true;
    while(readmore) {
        engine::EventType *eventType = eventBus.peak<engine::EventType>();
        switch (*eventType) {
            case engine::NEW_OBJECT: {
                auto event = eventBus.readNext<engine::NewObjectEvent>();
                objects.push_back(event->object);
                models[event->object] = event->model;
                engine::ObjectData objectData{nullptr, event->position, event->orientation, event->color};
                memoryManager->updateObject(event->object, objectData);
                break;
            }
            case engine::UPDATE_OBJECT: {
                auto event = eventBus.readNext<engine::UpdateObjectEvent>();
                engine::ObjectData objectData{nullptr, event->position, event->orientation, event->color};
                memoryManager->updateObject(event->object, objectData);
                break;
            }
            case engine::CHANGE_MODEL: {
                auto event = eventBus.readNext<engine::ChangeModelEvent>();
                models[event->object] = event->model;
                break;
            }
            case engine::COMMIT: {
                eventBus.readNext<engine::CommitEvent>();
                eventBus.commitReads();
                readmore = false;
                break;
            }
            default:
                throw runtime_error("Unkown event " + to_string(*eventType));
        }
    }
    

    Device device = context->getDevice();
    CommandBuffer commandBuffer = backFrame->commandBuffer;

    auto image_s = std::chrono::high_resolution_clock::now();
    uint32_t imageIdx = display->acquireNextFrameBufferId(backFrame->frameBufferAvailable);
    auto image_e = std::chrono::high_resolution_clock::now();

    cout << "image: " << chrono::duration <double, milli> (image_e - image_s).count() << endl;


    auto cpu_s = std::chrono::high_resolution_clock::now();
    auto renderPass_s = std::chrono::high_resolution_clock::now();

    CommandBufferBeginInfo commandBufferBeginInfo;
    commandBufferBeginInfo.flags = CommandBufferUsageFlagBits::eOneTimeSubmit;
    commandBuffer.begin(commandBufferBeginInfo);

    DescriptorBufferInfo bufferInfo0;
    bufferInfo0.buffer = memoryManager->objectMemory.buffer;
    bufferInfo0.offset = 0;
    bufferInfo0.range = VK_WHOLE_SIZE;

    WriteDescriptorSet writeDescriptor00;
    writeDescriptor00.dstSet = backFrame->descriptorSets[0];
    writeDescriptor00.dstBinding = 0;
    writeDescriptor00.dstArrayElement = 0;
    writeDescriptor00.descriptorCount = 1;
    writeDescriptor00.descriptorType = DescriptorType::eUniformBufferDynamic;
    writeDescriptor00.pBufferInfo = &bufferInfo0;


    DescriptorBufferInfo bufferInfo1;
    bufferInfo1.buffer = memoryManager->pvMemory.buffer;
    bufferInfo1.offset = 0;
    bufferInfo1.range = VK_WHOLE_SIZE;

    WriteDescriptorSet writeDescriptor10;
    writeDescriptor10.dstSet = backFrame->descriptorSets[1];
    writeDescriptor10.dstBinding = 0;
    writeDescriptor10.dstArrayElement = 0;
    writeDescriptor10.descriptorCount = 1;
    writeDescriptor10.descriptorType = DescriptorType::eUniformBuffer;
    writeDescriptor10.pBufferInfo = &bufferInfo1;

    auto preUpdate = std::chrono::high_resolution_clock::now();
    device.updateDescriptorSets(
            vector<WriteDescriptorSet>{
                    writeDescriptor00,
                    writeDescriptor10
            },
            {}
    );
    auto postUpdate = std::chrono::high_resolution_clock::now();

    cout << "updateSet: " << chrono::duration <double, milli> (postUpdate - preUpdate).count() << endl;


    memoryManager->copyMemory(commandBuffer);

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
                                     {backFrame->descriptorSets[1]}, {});

    Buffer modelBuffer = memoryManager->modelMemory.buffer;

    commandBuffer.bindPipeline(PipelineBindPoint::eGraphics, transparentRenderPass->getPipeline0());

    //TODO fix access to objects
    auto preloop = std::chrono::high_resolution_clock::now();
    for(engine::Object object: objects) {
        engine::ModelData *model = models[object];
        ModelData* modelData = static_cast<ModelData*> (model->renderData);

        DeviceSize offsetIndices = modelData->indexOffset;
        DeviceSize offsetVertices = modelData->vertexOffset;
        DeviceSize offsetUniform = object * memoryManager->sizeObjectUniform;

        commandBuffer.bindDescriptorSets(PipelineBindPoint::eGraphics, transparentRenderPass->getPipelineLayout(), 0,
                                         {backFrame->descriptorSets[0]}, {static_cast<uint32_t>(offsetUniform)});
        commandBuffer.bindVertexBuffers(0, 1, &modelBuffer, &offsetVertices);
        commandBuffer.bindIndexBuffer(modelBuffer, offsetIndices, IndexType::eUint32);

        commandBuffer.drawIndexed(model->getNbOfIndices(), 1, 0, 0, 0);
    }
    auto postloop = std::chrono::high_resolution_clock::now();
    cout << "loop: " << chrono::duration <double, milli> (postloop - preloop).count() << endl;


    //memoryManager->updateControllingObject(*scene.controllingObject);

    commandBuffer.nextSubpass(SubpassContents::eInline);
    commandBuffer.bindPipeline(PipelineBindPoint::eGraphics, transparentRenderPass->getPipeline1());

    /*auto object = *scene.controllingObject;
    ModelData* modelData = static_cast<ModelData *>(object.model->renderData);
    ObjectData* objectData = static_cast<ObjectData*>(object.renderData);

    DeviceSize offsetIndices = modelData->indexOffset;
    DeviceSize offsetVertices = modelData->vertexOffset;
    DeviceSize offsetUniform = objectData->dataOffset;

    uint32_t nbOfIndices = static_cast<uint32_t>(object.model->getIndices().size() * 3);

    commandBuffer.bindDescriptorSets(PipelineBindPoint::eGraphics, transparentRenderPass->getPipelineLayout(), 0,
                                     {backFrame->descriptorSets[0]}, {static_cast<uint32_t>(offsetUniform)});
    commandBuffer.bindVertexBuffers(0, 1, &modelBuffer, &offsetVertices);
    commandBuffer.bindIndexBuffer(modelBuffer, offsetIndices, IndexType::eUint32);
    commandBuffer.drawIndexed(nbOfIndices, 1, 0, 0, 0);
*/
    commandBuffer.nextSubpass(SubpassContents::eInline);
    //commandBuffer.bindPipeline(PipelineBindPoint::eGraphics, transparentRenderPass->getPipeline2());
    //commandBuffer.drawIndexed(nbOfIndices, 1, 0, 0, 0);


    commandBuffer.endRenderPass();
    commandBuffer.end();

    auto renderPass_e = std::chrono::high_resolution_clock::now();
    cout << "renderpass " << std::chrono::duration<double, milli>(renderPass_e - renderPass_s).count() << endl;


    auto display_s = std::chrono::high_resolution_clock::now();


    PipelineStageFlags waitStage = PipelineStageFlagBits::eTopOfPipe;

    SubmitInfo submitInfo;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &backFrame->frameBufferAvailable;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &backFrame->renderingDone;
    submitInfo.pWaitDstStageMask = &waitStage;

    //SDL_Delay(1); Simulate CPU intensive operations
    queue.submit(submitInfo, backFrame->frameAvailable);

    display->display(backFrame->renderingDone, imageIdx);
    auto display_e = std::chrono::high_resolution_clock::now();

    cout << "display " << std::chrono::duration<double, milli>(display_e - display_s).count() << endl;

    backFrame.swap(frontFrame);
    auto cpu_e = std::chrono::high_resolution_clock::now();
    cout << "CPU " << std::chrono::duration<double, milli>(cpu_e - cpu_s).count() << endl;


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

    frontFrame.reset();
    backFrame.reset();

    transparentRenderPass.reset();
    vertexShader.reset();
    fragmentShader.reset();

    display.reset();
    memoryManager.reset();
    context.reset();
}

void Renderer::loadModel(engine::ModelData& model) {
    memoryManager->loadModel(model);
}

engine::Object Renderer::newObject() {
    if(nextObject == memoryManager->maxNbOfObjects) {
        throw runtime_error("Max number of objects reached");
    }

    return nextObject++;
}



