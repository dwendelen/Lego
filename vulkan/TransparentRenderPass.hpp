//
// Created by xtrit on 4/11/17.
//

#ifndef LEGO_TRANSPARENTRENDERPASS_HPP
#define LEGO_TRANSPARENTRENDERPASS_HPP


#include <memory>
#include "vulkanExt.hpp"
#include "../engine/RenderingEngine.hpp"
#include "ModelData.hpp"
#include "SDL.h"
#include "MemoryManager.hpp"
#include "ObjectData.hpp"
#include "../engine/Object.hpp"
#include "Shader.hpp"
#include "Context.hpp"
#include "RenderPass.hpp"
#include "SimpleRenderPass.hpp"
#include "Display.hpp"

namespace vulkan {
    class TransparentRenderPass {
    private:
        vk::Device device;
        vk::ShaderModule vertexShader;
        vk::ShaderModule fragmentShader;

        vk::RenderPass opaqueRenderPass;
        vk::DescriptorSetLayout set0Layout;
        vk::DescriptorSetLayout set1Layout;

        vk::PipelineLayout pipelineLayout;
        vk::Pipeline pipeline0;
        vk::Pipeline pipeline1;

        void initRenderPass();
        void initSetLayouts();
        void initPipelineLayout();
        void initPipelines();
    public:
        TransparentRenderPass(vk::Device& device,  vk::ShaderModule vertexShader, vk::ShaderModule fragmentShader)
                : device(device)
                , vertexShader(vertexShader)
                , fragmentShader(fragmentShader){}

        void init();

        vk::RenderPass getRenderPass()  {
            return opaqueRenderPass;
        }

        vk::DescriptorSetLayout getSetLayout0() {
            return set0Layout;
        }

        vk::DescriptorSetLayout getSetLayout1() {
            return set1Layout;
        }

        vk::PipelineLayout getPipelineLayout() {
            return pipelineLayout;
        }

        vk::Pipeline getPipeline0() {
            return pipeline0;
        }

        vk::Pipeline getPipeline1() {
            return pipeline1;
        }

        ~TransparentRenderPass();
    };
};


#endif //LEGO_TRANSPARENTRENDERPASS_HPP