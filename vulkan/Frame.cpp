//
// Created by xtrit on 26/11/17.
//

#include <chrono>
#include <iostream>
#include "Frame.hpp"

void vulkan::Frame::init(uint32_t queueFamilyIndex, std::vector<vk::DescriptorSetLayout> descriptorSetLayouts) {
    createCommandBuffer(queueFamilyIndex);
    createDescriptorSet(descriptorSetLayouts);

    frameBufferAvailable = device.createSemaphore({});
    renderingDone = device.createSemaphore({});
    frameAvailable = device.createFence({vk::FenceCreateFlagBits::eSignaled});
}


void vulkan::Frame::createDescriptorSet(std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts) {
    std::vector<vk::DescriptorPoolSize> descriptorPoolSizes{
            {vk::DescriptorType::eUniformBufferDynamic, 1},
            {vk::DescriptorType::eUniformBuffer,        1},
    };

    vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo;
    descriptorPoolCreateInfo.maxSets = 2;
    descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size());
    descriptorPoolCreateInfo.pPoolSizes = descriptorPoolSizes.data();

    this->descriptorPool = device.createDescriptorPool(descriptorPoolCreateInfo);


    vk::DescriptorSetAllocateInfo descriptorSetAllocateInfo;
    descriptorSetAllocateInfo.descriptorPool = this->descriptorPool;
    descriptorSetAllocateInfo.descriptorSetCount = static_cast<uint32_t>(descriptorSetLayouts.size());
    descriptorSetAllocateInfo.pSetLayouts = descriptorSetLayouts.data();

    this->descriptorSets = this->device.allocateDescriptorSets(descriptorSetAllocateInfo);
}

void vulkan::Frame::createCommandBuffer(uint32_t queueFamilyIndex) {
    vk::CommandPoolCreateInfo commandPoolCreateInfo;
    commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;
    this->commandPool = this->device.createCommandPool(commandPoolCreateInfo);

    vk::CommandBufferAllocateInfo commandBufferInfo;
    commandBufferInfo.commandPool = this->commandPool;
    commandBufferInfo.commandBufferCount = 1;
    commandBufferInfo.level = vk::CommandBufferLevel::ePrimary;

    this->commandBuffer = this->device.allocateCommandBuffers(commandBufferInfo)[0];
}

void vulkan::Frame::waitForFrameAndResetResources() {
    auto r1 = std::chrono::high_resolution_clock::now();

    vk::Result result = device.waitForFences({frameAvailable}, VK_TRUE, 10000000000);
    if(result == vk::Result::eTimeout) {
        throw std::runtime_error("Device blocked");
    }
    auto r2 = std::chrono::high_resolution_clock::now();

    std::cout << "GPU " << std::chrono::duration<double, std::milli>(r2 - r1).count() << std::endl;


    device.resetFences({frameAvailable});
    device.resetCommandPool(commandPool, {});
}

vulkan::Frame::~Frame() {
    if (descriptorPool) {
        device.destroyDescriptorPool(descriptorPool);
    }
    if (commandPool) {
        device.destroyCommandPool(commandPool);
    }

    if(renderingDone) {
        device.destroySemaphore(renderingDone);
    }
    if(frameBufferAvailable) {
        device.destroySemaphore(frameBufferAvailable);
    }
    if(frameAvailable) {
        device.destroyFence(frameAvailable);
    }
}
