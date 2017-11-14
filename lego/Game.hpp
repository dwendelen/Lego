//
// Created by xtrit on 14/11/17.
//

#ifndef LEGO_GAME_HPP
#define LEGO_GAME_HPP

#include <memory>
#include "../engine/InputManager.hpp"
#include "GameInputMapping.hpp"
#include "BrickModel.hpp"

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
                  brick{4,2,3},
                  brick2 {2,2,3}
        {
            inputMapping = std::unique_ptr<GameInputMapping> (new GameInputMapping(*this));
        }

        void init();

    private:
        engine::Scene& scene;
        engine::RenderingEngine& renderingEngine;
        engine::InputManager& inputManager;

        std::unique_ptr<GameInputMapping> inputMapping;

        lego::BrickModel brick;
        lego::BrickModel brick2;
    };
}

#endif //LEGO_GAME_HPP
