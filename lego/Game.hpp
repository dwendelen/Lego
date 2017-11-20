//
// Created by xtrit on 14/11/17.
//

#ifndef LEGO_GAME_HPP
#define LEGO_GAME_HPP

#include <memory>
#include "../engine/InputManager.hpp"
#include "GameInputMapping.hpp"
#include "BrickModel.hpp"
#include "BrickCache.hpp"

namespace engine {
    class Scene;
    class RenderingEngine;
    class InputManager;
}

namespace lego {
    class Game {
    public:
        Game(engine::Scene& scene,
             engine::RenderingEngine& renderingEngine,
             engine::InputManager& inputManager
        )
                : renderingEngine(renderingEngine),
                  scene(scene),
                  inputManager(inputManager),
                  running(true),
                  brickCache(renderingEngine)
        {
            inputMapping = std::unique_ptr<GameInputMapping> (new GameInputMapping(*this));
        }

        void init();
        bool isRunning() {
            return running;
        }
        void tick(float secondsPassed);

        void placeBlock();
        void moveUnit(OVR::Vector3i translation);
        void rotate(OVR::Quatf rotation);
        void setVelocity(OVR::Vector3f velocity);
        void quit() {
            running = false;
        }

    private:
        engine::Scene& scene;
        engine::RenderingEngine& renderingEngine;
        engine::InputManager& inputManager;

        std::unique_ptr<GameInputMapping> inputMapping;

        BrickCache brickCache;

        bool running;
        OVR::Vector3f pseudoPosition;
        OVR::Vector3f velocity;

        void move(OVR::Vector3f translation);
    };
}

#endif //LEGO_GAME_HPP
