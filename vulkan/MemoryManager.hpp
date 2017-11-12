//
// Created by xtrit on 28/10/17.
//

#ifndef LEGO_MEMORYMANAGER_HPP
#define LEGO_MEMORYMANAGER_HPP

#include "vulkanExt.hpp"
#include "../engine/Math.hpp"

#include "ModelData.hpp"
#include "ObjectData.hpp"

namespace engine {
    class Model;
    class Object;
    class Camera;
}

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

        void loadModel(engine::Model& model);

        void loadObject(engine::Object& object);

        void updateControllingObject(engine::Object& controllingObject);

        void loadCamera(engine::Camera& camera);

        //void unload(engine::Model<ModelData> model);
        virtual ~MemoryManager();
    };
}

class UniformData {
public:
    OVR::Matrix4f mvp;
    OVR::Matrix4f rot;
    OVR::Vector4f color;
};

#endif //LEGO_MEMORYMANAGER_HPP
