//
// Created by xtrit on 1/11/17.
//

#include "Context.hpp"

#include <iostream>

using namespace std;
using namespace vk;

VkBool32 VulkanDebug_ReportCallback(
        VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT objType,
        uint64_t srcObject,
        size_t location,
        int32_t msgCode,
        const char *pLayerPrefix,
        const char *pMsg,
        void *pUserData) {
    cerr << pMsg << endl;
    return VK_FALSE;
}

template<typename T>
bool hasFlags(T actual, T expected) {
    return (actual & expected) == expected;
}


void checkExtensions(vector<char const *> required, vector<ExtensionProperties> extensions) {
    int nbOfMissingExtensions = 0;
    for (auto requiredExtension: required) {
        bool found = false;
        for (auto extension : extensions) {
            if (strcmp(requiredExtension, extension.extensionName) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            cerr << "Missing extension: " << requiredExtension << endl;
            nbOfMissingExtensions++;
        }
    }
    if (nbOfMissingExtensions != 0) {
        throw std::runtime_error(to_string(nbOfMissingExtensions) + " instanceExtensions missing!");
    }
}

void vulkan::Context::selectPhysicalDevice() {
    vector<PhysicalDevice> devices = instance.enumeratePhysicalDevices();
    for (PhysicalDevice device: devices) {
        if (device.getProperties().deviceType == PhysicalDeviceType::eDiscreteGpu) {
            physical = device;
            return;
        }
    }
    throw runtime_error("Could not find a discrete GPU");
}

void vulkan::Context::selectQueueFamilyIndex() {
    for (uint32_t i = 0; i < physical.getQueueFamilyProperties().size(); ++i) {
        auto queueFam = physical.getQueueFamilyProperties()[i];

        if (hasFlags(queueFam.queueFlags, QueueFlags(QueueFlagBits::eGraphics))) {
            queueFamilyIndex = i;
            return;
        }
    }

    throw runtime_error("No suitable queue family found");
}

void vulkan::Context::createDebugLogging() {
    DebugReportCallbackCreateInfoEXT debugCreateInfo;
    debugCreateInfo.flags = DebugReportFlagsEXT() | DebugReportFlagBitsEXT::eError | DebugReportFlagBitsEXT::eWarning;
    debugCreateInfo.pfnCallback = VulkanDebug_ReportCallback;
    debugReportCallback = instance.createDebugReportCallbackEXT(debugCreateInfo);
}

void vulkan::Context::load() {
    checkExtensions(instanceExtensions, enumerateInstanceExtensionProperties());

    ApplicationInfo appInfo;
    appInfo.pApplicationName = "Lego";
    appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 21);

    InstanceCreateInfo info{
            {},
            &appInfo,
            static_cast<uint32_t>(layers.size()), layers.data(),
            static_cast<uint32_t>(instanceExtensions.size()), instanceExtensions.data()
    };
    instance = createInstance(info);

    if(debug) {
        createDebugLogging();
    }

    selectPhysicalDevice();
    selectQueueFamilyIndex();

    checkExtensions(deviceExtensions, physical.enumerateDeviceExtensionProperties());
    checkFormats();

    vector<float> queuePriorities = {1.0};
    DeviceQueueCreateInfo queueInfo;
    queueInfo.queueCount = static_cast<uint32_t>(queuePriorities.size());
    queueInfo.queueFamilyIndex = queueFamilyIndex;
    queueInfo.pQueuePriorities = queuePriorities.data();

    DeviceCreateInfo deviceCreateInfo;
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueInfo;
    device = physical.createDevice(deviceCreateInfo);
}

void vulkan::Context::checkFormats() {
    FormatProperties depthBufferFormat = physical.getFormatProperties(Format::eD32Sfloat);
    if(!hasFlags(depthBufferFormat.optimalTilingFeatures, FormatFeatureFlags() | FormatFeatureFlagBits::eDepthStencilAttachment)) {
        throw runtime_error("Depth buffer format not supported");
    }

    FormatProperties srgbFormat = physical.getFormatProperties(Format::eB8G8R8A8Srgb);
    if(!hasFlags(srgbFormat.optimalTilingFeatures, FormatFeatureFlags() | FormatFeatureFlagBits::eColorAttachment | FormatFeatureFlagBits::eColorAttachmentBlend)) {
        throw runtime_error("SRGB not supported");
    }

    FormatProperties vertexFormat = physical.getFormatProperties(Format::eR32G32B32Sfloat);
    if(!hasFlags(vertexFormat.bufferFeatures, FormatFeatureFlags() | FormatFeatureFlagBits::eVertexBuffer)) {
        throw runtime_error("Format for vertex not supported");
    }
}

vulkan::Context::~Context() {
    if(device) {
        device.destroy();
    }
    if(debugReportCallback) {
        instance.destroyDebugReportCallbackEXT(debugReportCallback);
    }
    if (instance) {
        instance.destroy();
    }
}