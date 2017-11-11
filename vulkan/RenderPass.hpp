//
// Created by xtrit on 4/11/17.
//

#ifndef LEGO_RENDERPASS_HPP
#define LEGO_RENDERPASS_HPP


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
#include "Display.hpp"

namespace vulkan {
    class RenderPass {
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
        vk::Pipeline pipeline2;

        void initRenderPass();
        void initSetLayouts();
        void initPipelineLayout();
        void initPipelines();
    public:
        RenderPass(vk::Device& device,  vk::ShaderModule vertexShader, vk::ShaderModule fragmentShader)
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

        vk::Pipeline getPipeline2() {
            return pipeline2;
        }

        ~RenderPass();
    };
};


#endif //LEGO_RENDERPASS_HPP
