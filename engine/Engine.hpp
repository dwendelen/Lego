//
// Created by xtrit on 21/10/17.
//

#ifndef LEGO_ENGINE_HPP
#define LEGO_ENGINE_HPP


namespace engine {
    class RenderingEngine;

    class Engine {
        RenderingEngine& renderingEngine;
    public:
        explicit Engine(RenderingEngine& renderingEngine)
                : renderingEngine(renderingEngine) {}
        void init();
        virtual ~Engine() = default;
    };
}

#endif //LEGO_ENGINE_HPP
