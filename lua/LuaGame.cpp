//
// Created by xtrit on 06.01.18.
//

#include "LuaGame.hpp"

#include "../engine/InputManager.hpp"
#include "ScriptEngine.hpp"

void lua::LuaGame::init() {
    scriptEngine.init();
    inputManager.setMapping(inputMapping);
}

void lua::LuaGame::tick(float secondsPassed) {
    int nbOfTicks = 1;//static_cast<int>(secondsPassed * 100);
    for(int i = 0; i < nbOfTicks; i++) {
        update(inputMapping.input);
    }
}

void lua::LuaGame::update(engine::Input &input) {
    scriptEngine.update(input);
}
