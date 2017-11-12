//
// Created by xtrit on 28/10/17.
//

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


void vulkan::MemoryManager::init() {
    MemoryPropertyFlags requiredProperties
            = MemoryPropertyFlags()
              | MemoryPropertyFlagBits::eHostVisible
              | MemoryPropertyFlagBits::eHostCoherent;

    BufferCreateInfo createModelInfo;
    createModelInfo.size = 256 * 1024 * 1024;
    createModelInfo.sharingMode = SharingMode::eExclusive;
    createModelInfo.usage = BufferUsageFlags()
                            | BufferUsageFlagBits::eVertexBuffer
                            | BufferUsageFlagBits::eIndexBuffer;
    
    this->modelBuffer = device.createBuffer(createModelInfo);
    this->modelMemory = allocateMemory(this->modelBuffer, requiredProperties);


    BufferCreateInfo createObjectInfo;
    createObjectInfo.size = physicalDevice.getProperties().limits.maxUniformBufferRange;
    createObjectInfo.sharingMode = SharingMode::eExclusive;
    createObjectInfo.usage = BufferUsageFlagBits::eUniformBuffer;

    this->objectBuffer = device.createBuffer(createObjectInfo);
    this->objectMemory = this->allocateMemory(this->objectBuffer, requiredProperties);

    MemoryBlock firstBlock{};
    firstBlock.size = createObjectInfo.size;
    firstBlock.offset = 0;

    this->freeObjectMemory.push_back(firstBlock);

    //TODO FIX
    pvSize = physicalDevice.getProperties().limits.minUniformBufferOffsetAlignment;
    BufferCreateInfo createPvInfo;
    createPvInfo.size = pvSize;
    createPvInfo.sharingMode = SharingMode::eExclusive;
    createPvInfo.usage = BufferUsageFlagBits::eUniformBuffer;

    this->pvBuffer = device.createBuffer(createPvInfo);
    this->pvMemory = this->allocateMemory(pvBuffer, requiredProperties);

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

    uint8_t* writeLocation = static_cast<uint8_t *>(device.mapMemory(pvMemory, 0, pvSize));
    memcpy(writeLocation, &data, sizeof(data));
    device.unmapMemory(pvMemory);

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

    DeviceSize offsetVertices = modelOffset;
    DeviceSize offsetIndices = offsetVertices + sizeVertices;

    uint8_t* writeLocation = static_cast<uint8_t *>(device.mapMemory(modelMemory, modelOffset, sizeBoth));
    memcpy(writeLocation, model.getVerticesWithNormal().data(), sizeVertices);
    memcpy(writeLocation + sizeVertices, model.getIndices().data(), sizeIndices);
    device.unmapMemory(modelMemory);

    modelOffset += sizeBoth;

    //TODO Cleanup model data
    ModelData* modelData = new ModelData{};
    modelData->vertexOffset = offsetVertices;
    modelData->indexOffset = offsetIndices;

    model.renderData = modelData;
}

void vulkan::MemoryManager::loadCamera(engine::Camera& camera) {

}

DeviceSize vulkan::MemoryManager::allocateBlock(vk::DeviceSize size) {
    for(auto i = 0; i < freeObjectMemory.size(); i++) {
        if(freeObjectMemory[i].size >= size) {
            DeviceSize offset = freeObjectMemory[i].offset;
            freeObjectMemory[i].offset += size;
            freeObjectMemory[i].size -= size;

            return offset;
        }
    }

    throw runtime_error("Not enough memory for an extra object");
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

    DeviceSize offset = allocateBlock(alignedSize);

    uint8_t* writeLocation = static_cast<uint8_t *>(device.mapMemory(objectMemory, offset, alignedSize));
    memcpy(writeLocation, &data, sizeData);
    device.unmapMemory(objectMemory);

    ObjectData* objectData = new ObjectData{};
    objectData->dataOffset = offset;

    object.renderData = objectData;
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


    uint8_t* writeLocation = static_cast<uint8_t *>(device.mapMemory(objectMemory, objectData->dataOffset, alignedSize));
    memcpy(writeLocation, &data, sizeData);
    device.unmapMemory(objectMemory);
}

vulkan::MemoryManager::~MemoryManager() {
    if(staticMemory) {
        device.freeMemory(staticMemory);
    }

    if(pvBuffer) {
        device.destroyBuffer(pvBuffer);
    }

    if(pvMemory) {
        device.freeMemory(pvMemory);
    }

    if(modelBuffer) {
        device.destroyBuffer(modelBuffer);
    }
    
    if(modelMemory) {
        device.freeMemory(modelMemory);
    }

    if(objectBuffer) {
        device.destroyBuffer(objectBuffer);
    }
    if(objectMemory) {
        device.freeMemory(objectMemory);
    }
}



