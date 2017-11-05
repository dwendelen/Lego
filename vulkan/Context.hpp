//
// Created by xtrit on 1/11/17.
//

#ifndef LEGO_CONTEXT_HPP
#define LEGO_CONTEXT_HPP

#include "vulkanExt.hpp"

namespace vulkan {
    class Context {
    private:
        std::vector<char const *> layers;
        std::vector<char const *> instanceExtensions;
        std::vector<char const *> deviceExtensions;

        vk::Instance instance;
        vk::PhysicalDevice physical;
        vk::Device device;
        vk::DebugReportCallbackEXT debugReportCallback;
        uint32_t queueFamilyIndex;

        void createDebugLogging();
        void selectPhysicalDevice();
        void selectQueueFamilyIndex();
        void checkFormats();

    public:
        Context(std::vector<char const *> layers,
                std::vector<char const *> instanceExtensions,
                std::vector<char const *> deviceExtensions
        )
                :layers(layers)
                ,instanceExtensions(instanceExtensions)
                ,deviceExtensions(deviceExtensions){}

        vk::Instance getInstance() {
            return instance;
        }

        vk::PhysicalDevice getPhysical() {
            return physical;
        }

        vk::Device getDevice() {
            return device;
        }

        uint32_t getQueueFamilyIndex() {
            return queueFamilyIndex;
        }

        void load();

        virtual ~Context();

    };
}

#endif //LEGO_CONTEXT_HPP
