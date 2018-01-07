//
// Created by xtrit on 06.01.18.
//

#ifndef LEGO_INPUT_HPP
#define LEGO_INPUT_HPP

#include <SDL_gamecontroller.h>
#include <vector>

namespace engine {
    typedef std::pair<SDL_GameControllerAxis, float> AxisPair;
    class Input {
    public:
        std::vector<SDL_GameControllerButton> pressedButtons;
        std::vector<AxisPair> axes;
    };
}

#endif //LEGO_INPUT_HPP
