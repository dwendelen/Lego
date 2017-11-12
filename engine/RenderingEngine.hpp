//
// Created by xtrit on 21/10/17.
//

#ifndef LEGO_RENDERINGENGINE_HPP
#define LEGO_RENDERINGENGINE_HPP

namespace engine {
    class Scene;
    class Model;
    class Object;

    class RenderingEngine {
    public:
        virtual void render(Scene& scene) = 0;
        virtual void loadModel(engine::Model& model) = 0;
        virtual void loadObject(engine::Object& object) = 0;
    };
}
#endif //LEGO_RENDERINGENGINE_HPP_HPP
