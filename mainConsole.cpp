#include <iostream>
#include "engine/Scene.hpp"
#include "engine/Camera.hpp"
#include "lego/BrickModel.hpp"
#include "lego/TriangleModel.hpp"
#include "engine/Engine.hpp"
#include "engine/InputManager.hpp"
#include "lego/Game.hpp"
#include "console/ConsoleRenderer.hpp"

using namespace engine;
using namespace OVR;
using namespace console;

int main() {
    Scene scene;
    InputManager inputManager;
    ConsoleRenderer renderer;
    lego::Game game {scene, renderer, inputManager};

    engine::Engine engine {renderer, inputManager, scene, game};
    engine.init();
    engine.run();
}