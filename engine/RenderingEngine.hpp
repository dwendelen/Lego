//
// Created by xtrit on 21/10/17.
//

#ifndef LEGO_RENDERINGENGINE_HPP
#define LEGO_RENDERINGENGINE_HPP

#include "game_engine.hpp"

namespace engine {
    class RenderingEngine {
    public:
        virtual void init() = 0;
        virtual void render() = 0;
        virtual Object newObject() = 0;
        virtual void loadModel(engine::ModelData& model) = 0;
    };
}
#endif //LEGO_RENDERINGENGINE_HPP_HPP
