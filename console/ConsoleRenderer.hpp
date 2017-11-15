//
// Created by Wendelen Daan on 15/11/2017.
//

#ifndef LEGO_CONSOLERENDERER_HPP
#define LEGO_CONSOLERENDERER_HPP

#include "../engine/RenderingEngine.hpp"

namespace console {
    class ConsoleRenderer : public engine::RenderingEngine {
    public:
        void init() override;

        void render(engine::Scene &scene) override;

        void loadModel(engine::Model &model) override;

        void loadObject(engine::Object &object) override;
    };
}

#endif //LEGO_CONSOLERENDERER_HPP
