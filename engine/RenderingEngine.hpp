//
// Created by xtrit on 21/10/17.
//

#ifndef LEGO_RENDERINGENGINE_HPP
#define LEGO_RENDERINGENGINE_HPP

#include "Model.hpp"
#include "Object.hpp"

namespace engine {
    template<class ModelData, class ObjectData> class Scene;

    template<class ModelData, class ObjectData>
    class RenderingEngine {
    public:
        virtual void render(Scene<ModelData, ObjectData> &scene) = 0;
        virtual void loadModel(engine::Model<ModelData> &model) = 0;
        virtual void loadObject(engine::Object<ModelData, ObjectData> &object) = 0;
    };
}
#endif //LEGO_RENDERINGENGINE_HPP_HPP
