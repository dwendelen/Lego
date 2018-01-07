//
// Created by Wendelen Daan on 15/11/2017.
//

#ifndef LEGO_CONSOLERENDERER_HPP
#define LEGO_CONSOLERENDERER_HPP

#include "../engine/game_engine.hpp"
#include "../engine/RenderingEngine.hpp"

namespace console {
    class ConsoleRenderer : public engine::RenderingEngine {
    private:
        engine::EventBus& eventBus;
        engine::Object nextObject = 0;
    public:
        explicit ConsoleRenderer(engine::EventBus &eventBus)
                : eventBus(eventBus) {}

        void init() override;

        void render() override;

        engine::Object newObject() override {
            return nextObject++;
        }

        void loadModel(engine::ModelData &model) override;
    };
}

#endif //LEGO_CONSOLERENDERER_HPP
