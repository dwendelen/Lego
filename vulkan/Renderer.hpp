//
// Created by xtrit on 21/10/17.
//

#ifndef LEGO_VULKANRENDERER_HPP
#define LEGO_VULKANRENDERER_HPP

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


namespace vulkan {
    class Renderer : public engine::RenderingEngine<ModelData, ObjectData> {
    private:
        SDL_Window *window;
        std::unique_ptr<Context> context;
        vk::Queue queue;
        vk::SwapchainKHR swapChain;
        vk::CommandPool commandPool;
        //vk::CommandBuffer commandBuffer;

        std::unique_ptr<Shader> vertexShader;
        std::unique_ptr<Shader> fragmentShader;

        std::unique_ptr</*Simple*/RenderPass> renderPass;

        std::unique_ptr<MemoryManager> memoryManager;

        vk::DescriptorPool descriptorPool;

        std::vector<vk::Framebuffer> frameBuffers;
        std::vector<vk::ImageView> imageViews;
        vk::ImageView depthView;
        vk::Image depthBuffer;

    public:
        void init();
        void render(engine::Scene<ModelData, ObjectData> &scene) override;
        void loadModel(engine::Model<ModelData> &model) override;
        void loadObject(engine::Object<ModelData, ObjectData> &object) override;
        virtual ~Renderer();

        void createOpagePipeline();


    };
}

#endif //LEGO_VULKANRENDERER_HPP
