//
// Created by xtrit on 1/11/17.
//

#ifndef LEGO_SHADER_HPP
#define LEGO_SHADER_HPP

#include <string>
#include <vector>
#include "vulkanExt.hpp"

namespace vulkan {
    class Shader {
    private:
        std::string filename;
        vk::Device device;
        vk::ShaderModule shaderModule;

        std::vector<char> loadFile();

    public:
        explicit Shader(std::string filename, vk::Device device)
                : filename(filename)
                , device(device){}

        void load();

        vk::ShaderModule getShaderModule() {
            return shaderModule;
        }

        virtual ~Shader();
    };
}

#endif //LEGO_SHADER_HPP
