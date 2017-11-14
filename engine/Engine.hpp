//
// Created by xtrit on 21/10/17.
//

#ifndef LEGO_ENGINE_HPP
#define LEGO_ENGINE_HPP

#include <memory>

namespace lego {
    class Game;
}

namespace engine {
    class RenderingEngine;
    class InputManager;
    class Scene;

    class Engine {
        RenderingEngine& renderingEngine;
        InputManager& inputManager;
        Scene& scene;
        lego::Game& game;
    public:
        Engine(RenderingEngine &renderingEngine,
               InputManager &inputManager,
               Scene &scene,
               lego::Game &game
        )
                : renderingEngine(renderingEngine),
                  inputManager(inputManager),
                  scene(scene),
                  game(game)
        {}

        void init();
        void run();
        virtual ~Engine() = default;
    };
}

#endif //LEGO_ENGINE_HPP
