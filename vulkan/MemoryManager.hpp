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
    class Memory {
    public:
        vk::DeviceMemory memory {};
        vk::Buffer buffer;
        vk::DeviceSize used = 0;
        vk::DeviceSize size = 0;
    };
    class MemoryManager {
    private:
        vk::PhysicalDevice physicalDevice;
        vk::Device device;

        vk::DeviceMemory staticMemory;

        uint32_t getMemoryType(uint32_t memoryTypeBitsRequirement, vk::MemoryPropertyFlags requiredProperties);
        vk::DeviceMemory allocateMemory(vk::Buffer buffer, vk::MemoryPropertyFlags requiredProperties);
        Memory createMemory(vk::BufferUsageFlags bufferUsageFlags, vk::DeviceSize size, vk::MemoryPropertyFlags requiredProperties);

    public:
        Memory modelMemory;
        Memory modelStagingMemory;

        Memory objectMemory;
        Memory pvMemory;

        MemoryManager(vk::Device device, vk::PhysicalDevice physicalDevice)
                : device(device)
                , physicalDevice(physicalDevice)
        {}

        void init();

        void allocateStatic(std::vector<vk::Image> images);

        void loadModel(engine::Model& model);

        void loadObject(engine::Object& object);

        void updateControllingObject(engine::Object& controllingObject);

        void loadCamera(engine::Camera& camera);

        void copyMemory(vk::CommandBuffer& commandBuffer);

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
