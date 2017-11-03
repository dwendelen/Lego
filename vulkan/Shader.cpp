//
// Created by xtrit on 1/11/17.
//

#include "Shader.hpp"

#include <iostream>
#include <fstream>

using namespace std;
using namespace vk;

void vulkan::Shader::load() {
    vector<char> vertexCode = loadFile();

    ShaderModuleCreateInfo shaderModuleCreateInfo;
    shaderModuleCreateInfo.codeSize = vertexCode.size();
    shaderModuleCreateInfo.pCode = reinterpret_cast<uint32_t *>(vertexCode.data());

    shaderModule = device.createShaderModule(shaderModuleCreateInfo);
}

vector<char> vulkan::Shader::loadFile() {
    ifstream t(filename, ios::ate | ios::binary);
    if (!t.is_open()) {
        throw runtime_error("Could not read " + filename);
    }
    size_t size = static_cast<size_t>(t.tellg());
    vector<char> buffer(size);
    t.seekg(0);
    t.read(buffer.data(), size);
    t.close();
    return buffer;
}

vulkan::Shader::~Shader() {
    if(shaderModule) {
        device.destroyShaderModule(shaderModule);
    }
}
