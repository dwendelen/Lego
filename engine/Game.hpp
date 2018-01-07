//
// Created by xtrit on 06.01.18.
//

#ifndef LEGO_GAME_HPP
#define LEGO_GAME_HPP

#include "game_engine.hpp"

namespace engine {
    class Game {
    public:
        virtual void init() = 0;
        virtual void tick(float secondsPassed) = 0;
        virtual void update(Input& input) = 0;
        virtual bool isRunning() = 0;
        virtual ~Game() = default;
    };
}

#endif //LEGO_GAME_HPP
