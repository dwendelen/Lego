//
// Created by xtrit on 26/11/17.
//

#ifndef LEGO_FRAME_HPP
#define LEGO_FRAME_HPP

#include "vulkanExt.hpp"

namespace vulkan {
    class Frame {
    public:
        vk::Device device;

        vk::CommandPool commandPool;
        vk::CommandBuffer commandBuffer;

        vk::DescriptorPool descriptorPool;
        std::vector<vk::DescriptorSet> descriptorSets;

        vk::Semaphore frameBufferAvailable;
        vk::Semaphore renderingDone;
        vk::Fence frameAvailable;

        Frame(vk::Device device)
                : device(device) {}

        void init(uint32_t queueFamilyIndex, std::vector<vk::DescriptorSetLayout> descriptorSetLayouts);
        void waitForFrameAndResetResources();

        virtual ~Frame();

    private:
        void createCommandBuffer(uint32_t queueFamilyIndex);
        void createDescriptorSet(std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts);
    };
}
#endif //LEGO_FRAME_HPP
