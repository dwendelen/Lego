//
// Created by xtrit on 28/10/17.
//

#ifndef LEGO_MEMORYMANAGER_HPP
#define LEGO_MEMORYMANAGER_HPP

#include "vulkanExt.hpp"
#include "../engine/Model.hpp"
#include "ModelData.hpp"
#include "ObjectData.hpp"
#include "../engine/Object.hpp"
#include "../engine/Camera.hpp"

namespace vulkan {
    class MemoryBlock {
    public:
        vk::DeviceSize offset;
        vk::DeviceSize size;
    };
    class MemoryManager {
    private:
        vk::PhysicalDevice physicalDevice;
        vk::Device device;

        vk::DeviceMemory staticMemory;

        vk::DeviceMemory modelMemory;
        vk::Buffer modelBuffer;
        vk::DeviceSize modelBufferSize;
        vk::DeviceSize modelOffset;

        vk::DeviceMemory objectMemory;
        vk::Buffer objectBuffer;
        vk::DeviceSize objectBufferSize;
        std::vector<MemoryBlock> freeObjectMemory;

        vk::DeviceMemory pvMemory;
        vk::Buffer pvBuffer;
        vk::DeviceSize pvSize;

        uint32_t getMemoryType(uint32_t memoryTypeBitsRequirement, vk::MemoryPropertyFlags requiredProperties);
        vk::DeviceMemory allocateMemory(vk::Buffer buffer, vk::MemoryPropertyFlags requiredProperties);
        vk::DeviceSize allocateBlock(vk::DeviceSize size);

    public:
        MemoryManager(vk::Device device, vk::PhysicalDevice physicalDevice)
                : device(device)
                , physicalDevice(physicalDevice)
                , modelOffset(0)
        {}

        vk::Buffer getModelBuffer() {
            return modelBuffer;
        }

        vk::Buffer getObjectBuffer() {
            return objectBuffer;
        }

        vk::Buffer getPvBuffer() {
            return pvBuffer;
        }

        void init();

        void allocateStatic(std::vector<vk::Image> images);

        void loadModel(engine::Model<ModelData> &model);

        void loadObject(engine::Object<ModelData, ObjectData> &object);

        void loadCamera(engine::Camera& camera);

        //void unload(engine::Model<ModelData> model);
        virtual ~MemoryManager();
    };
}

#endif //LEGO_MEMORYMANAGER_HPP
