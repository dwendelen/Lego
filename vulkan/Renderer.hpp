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
#include "Frame.hpp"

namespace vulkan {
    class ModelData;
    class ObjectData;

    class Renderer : public engine::RenderingEngine {
    private:
        bool debug;

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
    public:
        Renderer(bool debug)
                : debug(debug)
                {}
        void init() override;
        void render(engine::Scene& scene) override;
        void loadModel(engine::Model& model) override;
        void loadObject(engine::Object& object) override;
        virtual ~Renderer();

        void createOpagePipeline();
    };
}

#endif //LEGO_VULKANRENDERER_HPP
