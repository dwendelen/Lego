//
// Created by xtrit on 07.01.18.
//

#ifndef LEGO_GAME_ENGINE_HPP
#define LEGO_GAME_ENGINE_HPP

#include <cstdint>

namespace engine {
    typedef uint32_t Object;
    typedef uint32_t Model;

    class Camera;
    class Engine;
    class EventBus;
    class Game;
    class Input;
    class InputGeneratingInputMapping;
    class InputManager;
    class InputMapping;
    class ModelData;
    class ObjectData;
    class RenderingEngine;
}

#endif //LEGO_GAME_ENGINE_HPP
