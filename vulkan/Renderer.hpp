//
// Created by xtrit on 21/10/17.
//

#ifndef LEGO_VULKANRENDERER_HPP
#define LEGO_VULKANRENDERER_HPP

#include "vulkanExt.hpp"
#include "vulkanRenderer.hpp"

#include "../engine/game_engine.hpp"
#include "../engine/RenderingEngine.hpp"
#include "Context.hpp"
#include "Shader.hpp"
#include "Display.hpp"
#include "RenderPass.hpp"
#include "MemoryManager.hpp"
#include "Frame.hpp"

namespace vulkan {

    class Renderer : public engine::RenderingEngine {
    private:
        bool debug;
        engine::EventBus& eventBus;

        std::unique_ptr<Context> context;
        std::unique_ptr<Display> display;
        vk::Queue queue;

        std::unique_ptr<Shader> vertexShader;
        std::unique_ptr<Shader> fragmentShader;
        std::unique_ptr<RenderPass> transparentRenderPass;
        std::unique_ptr<MemoryManager> memoryManager;


        std::vector<vk::Framebuffer> frameBuffers;
        std::vector<vk::ImageView> imageViews;
        vk::ImageView depthView;
        vk::Image depthBuffer;

        std::unique_ptr<Frame> frontFrame;
        std::unique_ptr<Frame> backFrame;

        engine::Object nextObject = 0;
        std::vector<engine::ModelData*> models{1024};
        std::vector<engine::Object> objects;
    public:
        Renderer(bool debug, engine::EventBus& eventBus)
                : debug(debug)
                , eventBus(eventBus)
                {}
        void init() override;
        void render() override;
        engine::Object newObject() override;
        void loadModel(engine::ModelData& model) override;
        virtual ~Renderer();

        void createOpagePipeline();
    };
}

#endif //LEGO_VULKANRENDERER_HPP
