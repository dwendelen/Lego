//
// Created by xtrit on 21/10/17.
//

#ifndef LEGO_ENGINE_HPP
#define LEGO_ENGINE_HPP

#include "game_engine.hpp"
#include <memory>

namespace lego {
    class LegoGame;
}

namespace engine {
    class Engine {
    private:
        RenderingEngine& renderingEngine;
        InputManager& inputManager;
        Game& game;
        uint32_t lastTick;
    public:
        Engine(RenderingEngine &renderingEngine,
               InputManager &inputManager,
               Game &game
        )
                : renderingEngine(renderingEngine),
                  inputManager(inputManager),
                  game(game)
        {}

        void init();
        void run();
        virtual ~Engine() = default;
    };
}

#endif //LEGO_ENGINE_HPP
