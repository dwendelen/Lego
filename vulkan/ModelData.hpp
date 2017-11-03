//
// Created by xtrit on 21/10/17.
//

#ifndef LEGO_VULKANDATA_HPP
#define LEGO_VULKANDATA_HPP

#include "vulkanExt.hpp"

namespace vulkan {
    class ModelData {
    public:
        vk::DeviceSize vertexOffset;
        vk::DeviceSize indexOffset;
    };
}

#endif //LEGO_VULKANDATA_HPP
