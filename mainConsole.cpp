#include <iostream>
#include "engine/Camera.hpp"
#include "lego/BrickModel.hpp"
#include "lego/TriangleModel.hpp"
#include "engine/Engine.hpp"
#include "engine/InputManager.hpp"
#include "console/ConsoleRenderer.hpp"
#include "engine/EventBus.hpp"
#include "lua/ScriptEngine.hpp"
#include "lua/LuaGame.hpp"
#include "lego/BrickCache.hpp"

using namespace engine;
using namespace OVR;
using namespace console;
using namespace lua;

int main() {
    InputManager inputManager;
    EventBus eventBus;
    ConsoleRenderer renderer {eventBus};

    lego::BrickCache brickCache{renderer};

    ScriptEngine scriptEngine{eventBus, brickCache, renderer};
    LuaGame luaGame {renderer, inputManager, scriptEngine};

    engine::Engine engine {renderer, inputManager, luaGame};
    engine.init();
    engine.run();
}