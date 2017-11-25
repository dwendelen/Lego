//
// Created by xtrit on 14/11/17.
//

#ifndef LEGO_GAME_HPP
#define LEGO_GAME_HPP

#include <memory>
#include "../engine/InputManager.hpp"
#include "ControlBrickInputMapping.hpp"
#include "BrickModel.hpp"
#include "BrickCache.hpp"
#include "ChangeModelInputMapping.hpp"

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
            controllBrickInputMapping = std::unique_ptr<ControlBrickInputMapping> (new ControlBrickInputMapping(*this));
            changeModelInputMapping = std::unique_ptr<ChangeModelInputMapping> (new ChangeModelInputMapping(*this));
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
        void setModelVelocity(OVR::Vector3f velocity);
        void quit() {
            running = false;
        }
        void changeBrick();

        void controlBrick();
        void changeColor(int delta);

    private:
        engine::Scene& scene;
        engine::RenderingEngine& renderingEngine;
        engine::InputManager& inputManager;

        std::unique_ptr<ControlBrickInputMapping> controllBrickInputMapping;
        std::unique_ptr<ChangeModelInputMapping> changeModelInputMapping;

        BrickCache brickCache;

        bool running;
        OVR::Vector3f pseudoPosition;
        OVR::Vector3f velocity;

        OVR::Vector3f brickDimensions;
        BrickDimension currentBrickDimensions;
        OVR::Vector3f modelVelocity;
        int colorIndex = 0;

        void move(OVR::Vector3f translation);
    };
}

#endif //LEGO_GAME_HPP
