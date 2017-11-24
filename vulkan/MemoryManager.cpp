//
// Created by xtrit on 28/10/17.
//

#include <iostream>
#include "MemoryManager.hpp"
#include "../engine/Object.hpp"
#include "../engine/Model.hpp"
#include "../engine/Camera.hpp"

using namespace vk;
using namespace std;
using namespace OVR;

DeviceSize align(DeviceSize offset, DeviceSize alignment) {
    DeviceSize comesShort = offset % alignment;
    if (comesShort == 0) {
        return offset;
    } else {
        DeviceSize padding = alignment - comesShort;
        return offset + padding;
    }
}

uint32_t vulkan::MemoryManager::getMemoryType(
        uint32_t memoryTypeBitsRequirement,
        MemoryPropertyFlags requiredProperties
) {
    PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();

    for (uint32_t memoryIndex = 0; memoryIndex < memoryProperties.memoryTypeCount; ++memoryIndex) {
        uint32_t memoryTypeBits = (1u << memoryIndex);
        bool isRequiredMemoryType = (memoryTypeBitsRequirement & memoryTypeBits) != 0;

        MemoryPropertyFlags properties = memoryProperties.memoryTypes[memoryIndex].propertyFlags;
        bool hasRequiredProperties = (properties & requiredProperties) == requiredProperties;

        if (isRequiredMemoryType && hasRequiredProperties)
            return memoryIndex;
    }

    throw runtime_error("Could not find memory with memoryTypeBitRequirements " + to_string(memoryTypeBitsRequirement) +
                        " and required property " + to_string(requiredProperties));
}

vk::DeviceMemory vulkan::MemoryManager::allocateMemory(vk::Buffer buffer, MemoryPropertyFlags requiredProperties) {
    MemoryRequirements requirements = device.getBufferMemoryRequirements(buffer);

    MemoryAllocateInfo memoryAllocateInfo;
    memoryAllocateInfo.allocationSize = requirements.size;
    memoryAllocateInfo.memoryTypeIndex = getMemoryType(requirements.memoryTypeBits, requiredProperties);

    DeviceMemory memory = device.allocateMemory(memoryAllocateInfo);

    device.bindBufferMemory(buffer, memory, 0);

    return memory;
}

vulkan::Memory vulkan::MemoryManager::createMemory(BufferUsageFlags bufferUsageFlags, DeviceSize size, vk::MemoryPropertyFlags requiredProperties) {
    BufferCreateInfo bufferCreateInfo;
    bufferCreateInfo.size = size;
    bufferCreateInfo.sharingMode = SharingMode::eExclusive;
    bufferCreateInfo.usage = bufferUsageFlags;

    Buffer buffer = device.createBuffer(bufferCreateInfo);
    DeviceMemory deviceMemory = allocateMemory(buffer, requiredProperties);
    
    Memory memory{};
    memory.size = size;
    memory.used = 0;
    memory.buffer = buffer;
    memory.memory = deviceMemory;

    return memory;
}



void vulkan::MemoryManager::init() {
    MemoryPropertyFlags hostRequirements
            = MemoryPropertyFlags()
              | MemoryPropertyFlagBits::eHostVisible
              | MemoryPropertyFlagBits::eHostCoherent;
    MemoryPropertyFlags deviceRequirements
            = MemoryPropertyFlagBits::eDeviceLocal;
    
    
    BufferUsageFlags modelBufferUsage
            = BufferUsageFlags()
                | BufferUsageFlagBits::eVertexBuffer
                | BufferUsageFlagBits::eIndexBuffer;
    BufferUsageFlagBits objectBufferUsage
            = BufferUsageFlagBits::eUniformBuffer;
    BufferUsageFlagBits pvBufferUsage
            = BufferUsageFlagBits::eUniformBuffer;

    DeviceSize modelBufferSize = 256 * 1024 * 1024;
    DeviceSize objectBufferSize = physicalDevice.getProperties().limits.maxUniformBufferRange;
    DeviceSize pvBufferSize = max(physicalDevice.getProperties().limits.minUniformBufferOffsetAlignment, sizeof(Matrix4f));


    this->modelMemory = createMemory(modelBufferUsage | BufferUsageFlagBits::eTransferDst, modelBufferSize, deviceRequirements);
    this->modelStagingMemory = createMemory(modelBufferUsage | BufferUsageFlagBits::eTransferSrc, modelBufferSize, hostRequirements);
    this->objectMemory = createMemory(objectBufferUsage, objectBufferSize, hostRequirements);
    this->pvMemory = createMemory(pvBufferUsage, pvBufferSize, hostRequirements);
    

//TODO FIX
    Vector3f x = Vector3f(1.0f,  0.0f,  0.0f); // Right
    Vector3f y = Vector3f(0.0f, -1.0f,  0.0f);
    Vector3f z = Vector3f(0.0f,  0.0f, -1.0f);  // Forward

    Quatf viewq {Vector3f{1,0,0},3.14/4};
    Matrix4f view2(	x.x, x.y, x.z, 0,
                      y.x, y.y, y.z, 0,
                      z.x, z.y, z.z, 0,
                      0  , 0  , 0  , 1);
    Matrix4f view {viewq};

    Matrix4f proj;
    float tanHalfFov = tan(0.15f);

    proj.M[0][0] = 0.75 / tanHalfFov;
    proj.M[1][1] = 1 / tanHalfFov;
    proj.M[2][2] = 10.0f / (10.0f - 0.05f);
    proj.M[2][3] = (0.05f * 10.0f) / (0.05f - 10.0f);
    proj.M[3][2] = 1;
    proj.M[3][3] = 0;
    Matrix4f pv = proj * view * view2;
    /*
    Matrix4f view = Matrix4f::LookAtRH(Vector3f(0, 0, 0), Vector3f(0, 0, -1), Vector3f(0, 1, 0));
    Matrix4f proj = Matrix4f::PerspectiveRH(0.3f, 4.0f/3.0f, 0.05f, 10.0f);
    Matrix4f pv = proj * view;*/

    Matrix4f data = pv.Transposed();

    uint8_t* writeLocation = static_cast<uint8_t *>(device.mapMemory(pvMemory.memory, 0, pvMemory.size));
    memcpy(writeLocation, &data, sizeof(data));
    device.unmapMemory(pvMemory.memory);
}


void vulkan::MemoryManager::allocateStatic(vector<Image> images) {
    uint32_t memoryBits = numeric_limits<uint32_t>::max();
    DeviceSize offsetNextImage = 0;

    for (Image image: images) {
        MemoryRequirements requirements = device.getImageMemoryRequirements(image);

        memoryBits &= requirements.memoryTypeBits;
        DeviceSize offsetCurrentImage = align(offsetNextImage, requirements.alignment);
        offsetNextImage = offsetCurrentImage + requirements.size;
    }

    MemoryAllocateInfo memoryAllocateInfo;
    memoryAllocateInfo.allocationSize = offsetNextImage;
    memoryAllocateInfo.memoryTypeIndex = getMemoryType(memoryBits, MemoryPropertyFlagBits::eDeviceLocal);

    this->staticMemory = device.allocateMemory(memoryAllocateInfo);
    
    DeviceSize offset = 0;
    for (Image image: images) {
        MemoryRequirements requirements = device.getImageMemoryRequirements(image);

        DeviceSize alignedOffset = align(offset, requirements.alignment);

        device.bindImageMemory(image, staticMemory, alignedOffset);

        offset = alignedOffset + requirements.size;
    }
}

void vulkan::MemoryManager::loadModel(engine::Model& model) {
    DeviceSize sizeVertices = model.getVerticesWithNormal().size() * sizeof(OVR::Vector3f);
    DeviceSize sizeIndices = model.getIndices().size() * sizeof(Vector3ui);

    DeviceSize sizeBoth = sizeVertices + sizeIndices;


    DeviceSize offsetVertices = modelStagingMemory.used;
    DeviceSize offsetIndices = offsetVertices + sizeVertices;
    DeviceSize newUsed = offsetIndices + sizeIndices;

    if(newUsed > modelStagingMemory.size) {
        throw runtime_error("Not enough model memory");
    }

    uint8_t* writeLocation = static_cast<uint8_t *>(device.mapMemory(modelStagingMemory.memory, modelStagingMemory.used, sizeBoth));
    memcpy(writeLocation, model.getVerticesWithNormal().data(), sizeVertices);
    memcpy(writeLocation + sizeVertices, model.getIndices().data(), sizeIndices);
    device.unmapMemory(modelStagingMemory.memory);

    modelStagingMemory.used = newUsed;

    //TODO Cleanup model data
    ModelData* modelData = new ModelData{};
    modelData->vertexOffset = offsetVertices;
    modelData->indexOffset = offsetIndices;

    model.renderData = modelData;
}

void vulkan::MemoryManager::loadCamera(engine::Camera& camera) {

}


void vulkan::MemoryManager::loadObject(engine::Object& object) {
    Matrix4<float> orientation = Matrix4f(object.orientation);
    Matrix4f model = Matrix4f::Translation(object.position) * orientation;

    UniformData data = {
        model.Transposed(),
        orientation.Transposed(),
        Vector4f(object.color, 1.0f)
    };

    DeviceSize sizeData = sizeof(data);
    DeviceSize alignedSize = align(sizeData, physicalDevice.getProperties().limits.minUniformBufferOffsetAlignment);

    DeviceSize newUsed = objectMemory.used + alignedSize;

    if(newUsed > objectMemory.size) {
        throw runtime_error("Not enough object memory");
    }

    uint8_t* writeLocation = static_cast<uint8_t *>(device.mapMemory(objectMemory.memory, objectMemory.used, alignedSize));
    memcpy(writeLocation, &data, sizeData);
    device.unmapMemory(objectMemory.memory);

    ObjectData* objectData = new ObjectData{};
    objectData->dataOffset = objectMemory.used;
    object.renderData = objectData;

    objectMemory.used = newUsed;
}

void vulkan::MemoryManager::updateControllingObject(engine::Object &controllingObject) {
    ObjectData* objectData = static_cast<ObjectData *>(controllingObject.renderData);

    Matrix4<float> orientation = Matrix4f(controllingObject.orientation);
    Matrix4f model = Matrix4f::Translation(controllingObject.position) * orientation;

    UniformData data = {
            model.Transposed(),
            orientation.Transposed(),
            Vector4f(controllingObject.color, 0.5f)
    };

    DeviceSize sizeData = sizeof(data);
    DeviceSize alignedSize = align(sizeData, physicalDevice.getProperties().limits.minUniformBufferOffsetAlignment);


    uint8_t* writeLocation = static_cast<uint8_t *>(device.mapMemory(objectMemory.memory, objectData->dataOffset, alignedSize));
    memcpy(writeLocation, &data, sizeData);
    device.unmapMemory(objectMemory.memory);
}


void vulkan::MemoryManager::copyMemory(vk::CommandBuffer &commandBuffer) {
    DeviceSize offset = modelMemory.used;
    DeviceSize size = modelStagingMemory.used - modelMemory.used;

    if(size == 0) {
        return;
    }

    BufferCopy bufferCopy;
    bufferCopy.srcOffset = offset;
    bufferCopy.dstOffset = offset;
    bufferCopy.size = size;

    commandBuffer.copyBuffer(modelStagingMemory.buffer, modelMemory.buffer, {bufferCopy});
    
    BufferMemoryBarrier bufferMemoryBarrier;
    bufferMemoryBarrier.buffer = modelMemory.buffer;
    bufferMemoryBarrier.offset = offset;
    bufferMemoryBarrier.size = size;
    bufferMemoryBarrier.srcAccessMask = AccessFlagBits::eMemoryWrite;
    bufferMemoryBarrier.dstAccessMask = AccessFlagBits::eIndexRead;
    bufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    bufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    commandBuffer.pipelineBarrier(PipelineStageFlagBits::eTransfer, PipelineStageFlagBits::eVertexInput, DependencyFlagBits::eByRegion, {}, {}, {});
}

vulkan::MemoryManager::~MemoryManager() {
    if(staticMemory) {
        device.freeMemory(staticMemory);
    }

    if(pvMemory.buffer) {
        device.destroyBuffer(pvMemory.buffer);
    }
    if(pvMemory.memory) {
        device.freeMemory(pvMemory.memory);
    }

    if(modelMemory.buffer) {
        device.destroyBuffer(modelMemory.buffer);
    }
    
    if(modelMemory.memory) {
        device.freeMemory(modelMemory.memory);
    }

    if(modelStagingMemory.buffer) {
        device.destroyBuffer(modelStagingMemory.buffer);
    }

    if(modelStagingMemory.memory) {
        device.freeMemory(modelStagingMemory.memory);
    }

    if(objectMemory.buffer) {
        device.destroyBuffer(objectMemory.buffer);
    }
    if(objectMemory.memory) {
        device.freeMemory(objectMemory.memory);
    }
}



