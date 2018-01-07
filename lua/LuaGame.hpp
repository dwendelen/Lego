//
// Created by xtrit on 06.01.18.
//

#ifndef LEGO_LUAGAME_HPP
#define LEGO_LUAGAME_HPP

#include "../engine/game_engine.hpp"
#include "../engine/InputGeneratingInputMapping.hpp"
#include "../engine/Game.hpp"
#include "ScriptEngine.hpp"

namespace lua {
    class ScriptEngine;

    class LuaGame: public engine::Game {
    public:
        LuaGame(engine::RenderingEngine &renderingEngine,
                engine::InputManager &inputManager,
                ScriptEngine& scriptEngine)
                : renderingEngine(renderingEngine),
                  inputManager(inputManager),
                  scriptEngine(scriptEngine){}

        void init() override;

        void tick(float secondsPassed) override;
        void update(engine::Input& input) override;
        bool isRunning() override {
            return !inputMapping.quit && !scriptEngine.quit;
        }
    private:
        engine::RenderingEngine &renderingEngine;
        engine::InputManager& inputManager;
        ScriptEngine& scriptEngine;
        engine::InputGeneratingInputMapping inputMapping;
    };
}

#endif //LEGO_LUAGAME_HPP
