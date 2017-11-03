//
// Created by xtrit on 1/11/17.
//

#ifndef LEGO_SIMPLERENDERPASS_HPP
#define LEGO_SIMPLERENDERPASS_HPP


#include "vulkanExt.hpp"

namespace vulkan {
    class SimpleRenderPass {
    private:
        vk::Device device;
        vk::ShaderModule vertexShader;
        vk::ShaderModule fragmentShader;

        vk::RenderPass opaqueRenderPass;
        vk::DescriptorSetLayout set0Layout;
        vk::DescriptorSetLayout set1Layout;

        vk::PipelineLayout pipelineLayout;
        vk::Pipeline opaquePipeline;

        void initRenderPass();
        void initSetLayouts();
        void initPipelineLayout();
        void initPipeline();
    public:
        SimpleRenderPass(vk::Device& device,  vk::ShaderModule vertexShader, vk::ShaderModule fragmentShader)
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

        vk::Pipeline getPipeline() {
            return opaquePipeline;
        }
        ~SimpleRenderPass();
    };
}

#endif //LEGO_SIMPLERENDERPASS_HPP
