#include <iostream>
#include "engine/Scene.hpp"
#include "engine/Camera.hpp"
#include "vulkan/ModelData.hpp"
#include "vulkan/Renderer.hpp"
#include "vulkan/MemoryManager.hpp"
#include "lego/BrickModel.hpp"
#include "lego/TriangleModel.hpp"
#include "engine/Engine.hpp"
#include "engine/InputManager.hpp"
#include "lego/Game.hpp"

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
    Scene scene;
    InputManager inputManager;
    Renderer renderer{debug};
    lego::Game game {scene, renderer, inputManager};

    engine::Engine engine {renderer, inputManager, scene, game};
    engine.init();
    engine.run();
}