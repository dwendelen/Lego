//
// Created by xtrit on 21/10/17.
//

#ifndef LEGO_VULKANRENDERER_HPP
#define LEGO_VULKANRENDERER_HPP

#include "vulkanExt.hpp"
#include "../engine/RenderingEngine.hpp"

#include "Context.hpp"
#include "Shader.hpp"
#include "Display.hpp"
#include "RenderPass.hpp"
#include "MemoryManager.hpp"

namespace vulkan {
    class ModelData;
    class ObjectData;

    class Renderer : public engine::RenderingEngine<ModelData, ObjectData> {
    private:
        std::unique_ptr<Context> context;
        std::unique_ptr<Display> display;
        vk::Queue queue;
        vk::CommandPool commandPool;

        std::unique_ptr<Shader> vertexShader;
        std::unique_ptr<Shader> fragmentShader;
        std::unique_ptr<RenderPass> transparentRenderPass;
        std::unique_ptr<MemoryManager> memoryManager;

        vk::DescriptorPool descriptorPool;

        std::vector<vk::Framebuffer> frameBuffers;
        std::vector<vk::ImageView> imageViews;
        vk::ImageView depthView;
        vk::Image depthBuffer;

        vk::Semaphore frameBufferReady;
        vk::Semaphore renderingDone;
        vk::Fence renderingDoneFence;
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
