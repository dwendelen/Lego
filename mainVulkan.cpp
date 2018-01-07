#include <iostream>
#include "engine/Camera.hpp"
#include "vulkan/ModelData.hpp"
#include "vulkan/Renderer.hpp"
#include "lego/BrickModel.hpp"
#include "lego/TriangleModel.hpp"
#include "engine/Engine.hpp"
#include "engine/InputManager.hpp"
#include "engine/EventBus.hpp"
#include "lua/LuaGame.hpp"
#include "lego/BrickCache.hpp"

using namespace engine;
using namespace vulkan;
using namespace OVR;

int main() {
#ifdef LEGO_DEBUG
    bool debug = true;
    std::cout << "Debug!" << std::endl;
#else
    bool debug = false;
#endif
    InputManager inputManager;
    EventBus eventBus;
    Renderer renderer{debug, eventBus};
    lego::BrickCache brickCache {renderer};
    lua::ScriptEngine scriptEngine{eventBus, brickCache, renderer };
    lua::LuaGame game {renderer, inputManager, scriptEngine };
    engine::Engine engine {renderer, inputManager, game};
    engine.init();
    engine.run();
}