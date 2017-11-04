//
// Created by xtrit on 21/10/17.
//

#ifndef LEGO_ENGINE_HPP
#define LEGO_ENGINE_HPP


namespace engine {
    template<class ModelData, class ObjectData>
    class RenderingEngine;

    template<class ModelData, class ObjectData>
    class Engine {
        RenderingEngine<ModelData, ObjectData> &renderingEngine;
    public:
        explicit Engine(RenderingEngine<ModelData, ObjectData> &renderingEngine)
                : renderingEngine(renderingEngine) {}
        void init();
        virtual ~Engine() = default;
    };
}

#endif //LEGO_ENGINE_HPP
